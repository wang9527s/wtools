#include "widget.h"

#include <QSystemTrayIcon>
#include <QIcon>
#include <QApplication>
#include <QVBoxLayout>
#include <QTabWidget>

#include "src/x11opacity/opacityWidget.h"
#include "src/drawer/drawerwidget.h"
#include "src/timerTask/timertask.h"
#include "src/tool/wtool.h"
#include "src/rss/rsswidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initIcon();
    setWindowTitle("title");
    QString icon = WTool::AppDir().absoluteFilePath("wtools.svg");
    setWindowIcon(QIcon(icon));
    setStyleSheet("QListWidget::item:selected{background:transparent;}");

    // 只设置Qt::Tool   点击QMessage::information的弹框后，软件退出
    // setWindowFlags(Qt::Tool);

    pTab = new QTabWidget(this);
    pTab->addTab(opacityWidget::instance(), "x11Opacity");
    pTab->addTab(RssWidget::instance(), u8"rss订阅");
    pTab->addTab(DrawerWidget::instance(), u8"抽屉");
    pTab->addTab(TimerTask::instance(), u8"定时任务");

    WTool::customTab(pTab);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pTab);
}

Widget::~Widget()
{
    delete pIcon;
}

void Widget::initIcon()
{
    // QIcon不存在的话，系统托盘会被放在收藏在抽屉中，看不到
    QString icon = WTool::AppDir().absoluteFilePath("wtools.svg");
    pIcon = new QSystemTrayIcon(QIcon(icon));
    pIcon->setVisible(true);
    connect(pIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        if (QSystemTrayIcon::Context == reason) {
            // 右击
            qApp->exit();
        }
        else if (QSystemTrayIcon::Trigger == reason) {
            // 左击
            this->setVisible(!isVisible());
        }
    });
}
