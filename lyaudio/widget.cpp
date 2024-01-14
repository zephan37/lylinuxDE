#include "widget.h"
#include "ui_widget.h"
#include "exec_linux_cmd/execlinuxcmd.h"

#include <QScreen>
#include <QTimer>
#include <QGraphicsOpacityEffect>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screen_msg = screen->availableGeometry();
    this->resize(screen_msg.width()/9, screen_msg.width()/9);
    this->setAttribute(Qt::WA_TranslucentBackground, true);

    ui->groupBox->setStyleSheet("background-color: rgba(255, 255, 255, 0.4); border-radius: 10px;");

    ui->groupBox_2->setStyleSheet("background-color: rgba(0, 0, 0, 0)");
    ui->groupBox_3->setStyleSheet("background-color: rgba(0, 0, 0, 0)");

    //icon
    ui->label->setPixmap(QPixmap(":/resource/resource/image/audio.png"));

    //time
    time_17us = new QTimer(this);
    time_1s = new QTimer(this);
    old_value = 0;
    current_value = 0;

    connect(time_17us, &QTimer::timeout, this, &Widget::update_process_value);
    time_17us->start(17);

    connect(time_1s, &QTimer::timeout, this, &Widget::close_widge);
    time_1s->start(1000);

}

void Widget::close_widge()
{
    this->close();
}

void Widget::update_process_value()
{
    QString cmd_result;
    QString cmd_error;
    exec_linux_cmd(QString("pamixer --get-volume"), cmd_result, cmd_error);
    ui->progressBar->setValue(cmd_result.toInt());
    current_value = cmd_result.toInt();

    if(current_value != old_value) {
        time_1s->stop();
        time_1s->start();
        old_value = current_value;
    }
}

Widget::~Widget()
{
    delete ui;
}

