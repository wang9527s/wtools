#include "iconwidget.h"
#include "ui_iconwidget.h"

#include <QDebug>
#include <QIcon>

IconWidget::IconWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IconWidget)
{
    ui->setupUi(this);
    connect(ui->path,&QLineEdit::returnPressed,this,&IconWidget::on_show_clicked);
}

IconWidget::~IconWidget()
{
    delete ui;
}

void IconWidget::on_show_clicked()
{
    const QString path = ui->path->text();
    /* tip
       QIcon::fromTheme("/opt/apps/x11opacity-wang/window.svg")和QIcon::fromTheme("computer")都可以正确加载图标

       QIcon("/opt/apps/x11opacity-wang/window.svg")可以正确加载图标，QIcon("computer")加载图标失败
    */
    QIcon icon=QIcon::fromTheme(path);
    ui->pix->setText("");
    if (icon.isNull()){
        ui->pix->setText("该icon不存在");
        return;
    }
    qInfo()<<path<<icon.isNull()<<QIcon::themeName()<<icon.availableSizes()<<icon.name();
    QPixmap pix;
        const int s = 8;
        const int l[s] = { 64,16, 18, 24, 32, 96, 128, 256 };
         qInfo()<<pix.isNull();
        for (int i(0); i != s; ++i){
            pix=icon.pixmap(QSize(l[i],l[i]));
            qInfo()<<pix.isNull()<<l[i];
            if(!pix.isNull()){
                break;
            }
        }


    ui->pix->setPixmap(pix);
}
