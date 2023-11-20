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
#include <QTextEdit>

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
    pInstructions->setText(u8"功能：\n"
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
    pOpalPath = new QLineEdit(this);
    pOpalPath->setText(mJsonConfig.value("opal_path").toString());

    mCreateFollowingOpml = new QPushButton(u8"创建OPML", this);
    mUpdateOpml = new QPushButton(u8"更新我的OPML", this);

    pEdit = new QTextEdit(this);

    QHBoxLayout *pH1 = new QHBoxLayout;
    pH1->addWidget(new QLabel(u8"rss服务器地址 "));
    pH1->addWidget(mRssService);

    QHBoxLayout *pH2 = new QHBoxLayout;
    pH2->addWidget(new QLabel(u8"UP主的用户id "));
    pH2->addWidget(mBilibiliVmid);

    QHBoxLayout *pH3 = new QHBoxLayout;
    pH3->addWidget(new QLabel(u8"opal路径 "));
    pH3->addWidget(pOpalPath);

    QHBoxLayout *pH4 = new QHBoxLayout;
    pH4->addWidget(mCreateFollowingOpml);
    pH4->addWidget(mUpdateOpml);

    QVBoxLayout *pLayout = new QVBoxLayout(this);
    pLayout->addWidget(pInstructions);
    pLayout->addSpacing(20);
    pLayout->addLayout(pH1);
    pLayout->addLayout(pH2);
    pLayout->addLayout(pH3);
    pLayout->addLayout(pH4);
    pLayout->addWidget(pEdit);

    connect(pOpalPath, &QLineEdit::returnPressed, this, [=] {
        mJsonConfig.insert("opal_path", pOpalPath->text());
        save_json();
    });
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
        WTool::sendNotice(u8"请输入vmid");
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

#ifdef Plat_Windows

    auto get_url_from_html = [](QString one) -> QString {
        QString tmp = one.split("xmlUrl").last();
        if (tmp.split('"').size() >= 3)
            return tmp.split('"')[1];

        return "";
    };
    QStringList urls_in_file;
    for (auto e : WTool::content(pOpalPath->text())) {
        QString url = get_url_from_html(e);
        if (url != "")
            urls_in_file << url;
    }

    QString res =
        RSSUpdateOPML::updateBilibili_win(rssSer,
                                          mJsonConfig.value("bilibili_vid").toString(),
                                          mJsonConfig.value("bilibili_opmlPath").toString());

    pEdit->clear();
    for (auto e : res.split('\n')) {
        QString url = get_url_from_html(e);
        if (!urls_in_file.contains(url))
            pEdit->append(e);
    }

#endif
#ifdef Linux
    RSSUpdateOPML::updateBilibili(rssSer,
                                  mJsonConfig.value("bilibili_vid").toString(),
                                  mJsonConfig.value("bilibili_opmlPath").toString());
#endif
}

void RssWidget::save_json()
{
    QString cfg = WConfig::ConfigDir + "/config.json";
    WTool::saveJsonToConfig(mJsonConfig, cfg);
}
