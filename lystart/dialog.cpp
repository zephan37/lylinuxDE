#include "dialog.h"
#include "ui_dialog.h"
#include <QScreen>
#include <QThread>
#include "exec_linux_cmd/execlinuxcmd.h"
#include <QString>
#include <QFont>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screen_msg = screen->availableGeometry();
    this->resize(screen_msg.width()/5, screen_msg.width()/15);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->pushButton->setMinimumHeight(this->width()*0.25);
    ui->pushButton_2->setMinimumHeight(this->width()*0.25);
    ui->pushButton_3->setMinimumHeight(this->width()*0.25);

    QFont font("Source Code Pro", 10, 800);
    ui->label->setText(QString("Logout"));
    ui->label->setFont(font);
    ui->label_2->setText(QString("Reboot"));
    ui->label_2->setFont(font);
    ui->label_3->setText(QString("Shutdown"));
    ui->label_3->setFont(font);

    ui->pushButton->setIcon(QIcon(":/image/resource/image/logout.svg"));
    ui->pushButton->setIconSize(QSize(40, 40));
    ui->pushButton_2->setIcon(QIcon(":/image/resource/image/restart.svg"));
    ui->pushButton_2->setIconSize(QSize(40, 40));
    ui->pushButton_3->setIcon(QIcon(":/image/resource/image/shutdown.svg"));
    ui->pushButton_3->setIconSize(QSize(40, 40));

    ui->groupBox->setStyleSheet("background-color: rgba(255, 255, 255, 0.67); border-radius: 10px;");
    ui->label->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    ui->label_2->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    ui->label_3->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    ui->pushButton->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    ui->pushButton_2->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    ui->pushButton_3->setStyleSheet("background-color: rgba(0, 0, 0, 0)");

    connect(ui->pushButton, &QPushButton::pressed, [=]() {
        ui->pushButton->setIconSize(QSize(20, 20));
    });
    connect(ui->pushButton, &QPushButton::released, [=]() {
        ui->pushButton->setIconSize(QSize(40, 40));
    });

    connect(ui->pushButton_2, &QPushButton::pressed, [=]() {
        ui->pushButton_2->setIconSize(QSize(20, 20));
    });
    connect(ui->pushButton_2, &QPushButton::released, [=]() {
        ui->pushButton_2->setIconSize(QSize(40, 40));
    });

    connect(ui->pushButton_3, &QPushButton::pressed, [=]() {
        ui->pushButton_3->setIconSize(QSize(20, 20));
    });
    connect(ui->pushButton_3, &QPushButton::released, [=]() {
        ui->pushButton_3->setIconSize(QSize(40, 40));
    });

    connect(ui->pushButton, &QPushButton::clicked, [=]() {
        this->hide();
        box = QMessageBox::question(this, "logout", "Logout ?", QMessageBox::Yes|QMessageBox::No);
        if(box == QMessageBox::No) {
            this->show();
        }
        else {
            QString res;
            QString error;
            exec_linux_cmd(QString("/usr/bin/stop_launch"), res, error);
        }

    });

    connect(ui->pushButton_2, &QPushButton::clicked, [=]() {
        this->hide();
        box = QMessageBox::question(this, "reboot", "Reboot ?", QMessageBox::Yes|QMessageBox::No);
        if(box == QMessageBox::No) {
            this->show();
        }
        else {
            QString res;
            QString error;
            exec_linux_cmd(QString("sudo reboot"), res, error);
        }
    });

    connect(ui->pushButton_3, &QPushButton::clicked, [=]() {
        this->hide();
        box = QMessageBox::question(this, "shutdown", "Shutdown ?", QMessageBox::Yes|QMessageBox::No);
        if(box == QMessageBox::No) {
            this->show();
        }
        else {
            QString res;
            QString error;
            exec_linux_cmd(QString("sudo shutdown -h now"), res, error);
        }
    });

}

Dialog::~Dialog()
{
    delete ui;
}

