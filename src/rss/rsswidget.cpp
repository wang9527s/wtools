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

RssWidget::RssWidget(QWidget *parent)
    : QWidget(parent)
{
    QString cfg = WConfig::ConfigDir + "/config.json";
    if (!QFileInfo::exists(cfg)) {
        mJsonConfig.insert("bilibili_vid", "");
        mJsonConfig.insert("bilibili_opmlPath", "");
        WTool::saveJsonToConfig(mJsonConfig, cfg);
    }
    mJsonConfig = WTool::getJsonFromConfig(cfg);

    QLabel *pInstructions = new QLabel(this);
    pInstructions->setText("功能：\n"
                           "1.获取UP主关注的用户id，并在桌面上生成一个opml文件。\n"
                           "2.更新我的opml文件\n"
                           "        如果本地不存在，则将数据插入“bilibili我关注的up主”行之后\n"
                           "配置文件：\n"
                           "        json文件位置：~/.config/wtools/config.json\n"
                           "        默认的UP主用户id：bilibili_vid的值\n"
                           "        功能2中，被更新的文件位置:bilibili_opmlPath的值\n"
                           "注意：因为接口限制，关注的用户超过250（5页），有部分数据无法获取。"
                           "");

    mRssService = new QLineEdit(this);
    mRssService->setPlaceholderText("127.0.0.1:1200");
    mBilibiliVmid = new QLineEdit(this);
    mBilibiliVmid->setText(mJsonConfig.value("bilibili_vid").toString());

    mCreateFollowingOpml = new QPushButton("创建OPML", this);
    mUpdateOpml = new QPushButton("更新我的OPML", this);

    QHBoxLayout *pH1 = new QHBoxLayout;
    pH1->addWidget(new QLabel("rss服务器地址 "));
    pH1->addWidget(mRssService);

    QHBoxLayout *pH2 = new QHBoxLayout;
    pH2->addWidget(new QLabel("UP主的用户id "));
    pH2->addWidget(mBilibiliVmid);

    QHBoxLayout *pH3 = new QHBoxLayout;
    pH3->addWidget(mCreateFollowingOpml);
    pH3->addWidget(mUpdateOpml);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pInstructions);
    pLayout->addSpacing(20);
    pLayout->addLayout(pH1);
    pLayout->addLayout(pH2);
    pLayout->addLayout(pH3);
    pLayout->addStretch();

    connect(mUpdateOpml, &QPushButton::clicked, this, &RssWidget::onUpdateBilibiliOPML);
    connect(mCreateFollowingOpml,
            &QPushButton::clicked,
            this,
            &RssWidget::onCreateBiibiliFollowingOPML);
}

void RssWidget::onCreateBiibiliFollowingOPML()
{
    const QString vmid = mBilibiliVmid->text();
    if (vmid == "") {
        WTool::sendNotice("请输入vmid");
        return;
    }

    QString rssSer =
        mRssService->text().isEmpty() ? mRssService->placeholderText() : mRssService->text();
    RSSUpdateOPML::createFollowingsOPML(rssSer, vmid);
}

void RssWidget::onUpdateBilibiliOPML()
{
    // QString("").isEmpty();          // returns true
    // QString("").isNull();           // returns false
    QString rssSer =
        mRssService->text().isEmpty() ? mRssService->placeholderText() : mRssService->text();

    RSSUpdateOPML::updateBilibili(rssSer,
                                  mJsonConfig.value("bilibili_vid").toString(),
                                  mJsonConfig.value("bilibili_opmlPath").toString());
}
