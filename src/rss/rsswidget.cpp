#include "rsswidget.h"
#include "ui_rsswidget.h"

#include "src/rss/rssupdateopml.h"
#include "src/tool/wtool.h"

RssWidget::RssWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::RssWidget)
{
    ui->setupUi(this);
}

RssWidget::~RssWidget()
{
    delete ui;
}

void RssWidget::on_createFollowingOPML_clicked()
{
    const QString vmid = ui->vmid->text();
    if(vmid==""){
        WTool::sendNotice("请输入vmid");
        return;
    }

    QString rssSer = ui->rssServer->text();
    if(rssSer==""){
        rssSer="127.0.0.1:1200";
    }


    RSSUpdateOPML::createFollowingsOPML(rssSer,vmid);
}

void RssWidget::on_updateBilibili_clicked()
{
    QString rssSer = ui->rssServer->text();
    if(rssSer==""){
        rssSer="127.0.0.1:1200";
    }

    RSSUpdateOPML::updateBilibili(rssSer);
}
