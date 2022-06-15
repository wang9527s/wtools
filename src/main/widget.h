#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
    class widget;
}

class QSystemTrayIcon;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();



private:
    void initIcon();

    Ui::widget * ui;
    QSystemTrayIcon * pIcon;
};

#endif // WIDGET_H
