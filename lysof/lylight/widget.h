#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void update_process_value();
    void close_widge();

private:
    Ui::Widget *ui;
    int current_value;
    int old_value;

    QTimer *time_17us;
    QTimer *time_1s;
};
#endif // WIDGET_H
