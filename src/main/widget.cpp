#include "widget.h"

#include <QSystemTrayIcon>
#include <QIcon>
#include <QApplication>
#include <QVBoxLayout>
#include <QTabWidget>

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    initIcon();
    setWindowTitle("title");
    setWindowIcon(QIcon("/opt/apps/wtools/wtools.svg"));
    setStyleSheet(
                "QListWidget::item:selected{background:transparent;}"
                );

    setWindowFlags(Qt::Tool);

    pTab = new QTabWidget(this);
    pTab->setCurrentIndex(pTab->count()-1);

    QVBoxLayout * pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pTab);
}

Widget::~Widget()
{
    delete pIcon;
}

void Widget::initIcon()
{
    pIcon=new QSystemTrayIcon(QIcon("/opt/apps/wtools/wtools.svg"));
    pIcon->setVisible(true);
    connect(pIcon,&QSystemTrayIcon::activated,[=](QSystemTrayIcon::ActivationReason reason){
        if (QSystemTrayIcon::Context==reason){
            //右击
            qApp->exit();
        } else if (QSystemTrayIcon::Trigger==reason){
            //左击
            this->setVisible(!isVisible());
        }
    });
}
