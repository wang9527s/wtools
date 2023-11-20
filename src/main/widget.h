#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QSystemTrayIcon;
class QTabWidget;
class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    void initIcon();

    QTabWidget *pTab;
    QSystemTrayIcon *pIcon;
};

#endif // WIDGET_H
