#include "widget.h"

#include "ui_widget.h"
#include <QSystemTrayIcon>
#include <QIcon>
#include <QApplication>
#include <QVBoxLayout>


#include "src/x11opacity/opacityWidget.h"
#include "src/icon/iconwidget.h"
#include "src/timerTask/timertask.h"
#include "src/tool/wtool.h"
#include "src/rss/rsswidget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::widget)
{
    initIcon();
    setWindowTitle("title");
    setWindowIcon(QIcon("/opt/apps/wtools/wtools.svg"));
    setStyleSheet(
                "QListWidget::item:selected{background:transparent;}"
                );

    setWindowFlags(Qt::Tool);

    ui->setupUi(this);
    ui->tabWidget->addTab(opacityWidget::instance(),"x11Opacity");
    ui->tabWidget->addTab(IconWidget::instance(),"展示QIcon");
    ui->tabWidget->addTab(TimerTask::instance(),"定时任务");

    if(WTool::isAuthor()){
        ui->tabWidget->addTab(RssWidget::instance(),"rss订阅");
    }

    ui->tabWidget->setCurrentIndex(ui->tabWidget->count()-1);

    WTool::customTab(ui->tabWidget);
}

Widget::~Widget()
{
    delete ui;
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
