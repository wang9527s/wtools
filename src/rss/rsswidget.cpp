#include "rsswidget.h"

#include "src/rss/rssupdateopml.h"
#include "src/tool/wtool.h"
#include "src/tool/wconfig.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QFileInfo>

RssWidget * RssWidget::instance(){
    static RssWidget * pIns =nullptr;
    if(pIns==nullptr){
        pIns=new RssWidget(nullptr);
    }
    return  pIns;
}

RssWidget::RssWidget(QWidget *parent)
    : QWidget(parent)
{
    mRssService=new QLineEdit(this);
    mRssService->setPlaceholderText("127.0.0.1:1200");
    mBilibiliVmid=new QLineEdit(this);
    mBilibiliVmid->setPlaceholderText("请输入要获取的UP主的vmid");

    mCreateFollowingOpml=new QPushButton("创建OPML",this);
    mUpdateOpml=new QPushButton("更新OPML",this);


    QHBoxLayout * pH1 =new QHBoxLayout;
    pH1->addWidget(new QLabel("rss服务器地址 "));
    pH1->addWidget(mRssService);

    QHBoxLayout * pH2 =new QHBoxLayout;
    pH2->addWidget(mBilibiliVmid);
    pH2->addWidget(mCreateFollowingOpml);

    QVBoxLayout * pLayout =new QVBoxLayout(this);
    pLayout->addLayout(pH1);
    pLayout->addWidget(mUpdateOpml);
    pLayout->addLayout(pH2);
    pLayout->addStretch();

    connect(mUpdateOpml,&QPushButton::clicked,this,&RssWidget::updateBilibili);
    connect(mCreateFollowingOpml,&QPushButton::clicked,this,&RssWidget::createFollowingOPML);
}

RssWidget::~RssWidget()
{

}

void RssWidget::createFollowingOPML()
{
    const QString vmid = mBilibiliVmid->text();
    if(vmid==""){
        WTool::sendNotice("请输入vmid");
        return;
    }

    QString rssSer = mRssService->text();
    if(rssSer==""){
        rssSer="127.0.0.1:1200";
    }


    RSSUpdateOPML::createFollowingsOPML(rssSer,vmid);
}

void RssWidget::updateBilibili()
{
    QString rssSer = mRssService->text();
    if(rssSer==""){
        rssSer="127.0.0.1:1200";
    }

    QJsonObject e = WTool::getJsonFromConfig(WConfig::ConfigDir + "/config.json");
    RSSUpdateOPML::updateBilibili(rssSer,e.value("bilibili_vid").toString(),e.value("bilibili_opmlPath").toString());
}
