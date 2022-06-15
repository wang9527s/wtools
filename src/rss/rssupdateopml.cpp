#include "rssupdateopml.h"

#include "src/tool/wtool.h"
#include "src/tool/wconfig.h"

#include <QDateTime>
#include <QDebug>
#include <QDir>

void RSSUpdateOPML::updateBilibili(const QString & rssServer)
{
    const QString opmlPath = QDir::homePath()+"/wwork/1.bin/4.AppImage/rss/2.opml/rss3.bilibili.opml";

    // opml文件中已经存在该id
    QString cmd1=QString(" egrep 'bilibili/user/video' %1|awk -F 'video/' '{print $2}' |cut -d '%2' -f 1")
            .arg(opmlPath).arg('"');
    QStringList existIds = WTool::runCmdResultWithPipe("/bin/bash",QStringList()
                                    <<"-c"
                                    <<cmd1
                                    ).split('\n');
    existIds.removeAll("");

    // 获取行号 bilibili我关注的up主
    int insertLineNumber = WTool::runCmdResultWithPipe("/bin/bash",QStringList()
                                <<"-c"
                                <<QString("grep -n bilibili我关注的up主 %1|cut -d ':' -f 1").arg(opmlPath)
                                ).toInt();

    //  1069577466   265657226
    QStringList lines = getFollowings(rssServer,"265657226");
//    qInfo()<<lines;
    for (auto oneRecord: lines){
        bool exist=false;
        for (auto vmid: existIds) {
            if (oneRecord.contains(vmid)){
                qInfo()<<"update exist"<<oneRecord<<vmid;
                exist = true;
                break;
            }
        }
        if(exist)
            continue;

        qInfo()<<"update insert";
        //插入数据
        WTool::runCmd(QString("sed -i '%1a %2' %3")
                      .arg(insertLineNumber++)
                      .arg(oneRecord)
                      .arg(opmlPath));
    }
}

void RSSUpdateOPML::updateBilibili_old(const QString &rssServer)
{
    const QString opmlPath = QDir::homePath()+"/wwork/1.bin/4.AppImage/rss/rss订阅.opml";
    // 清除 “bilibili关注” 文件夹
    WTool::runCmd(QString("%1 %2 %3 %4")
                  .arg(WConfig::BashDir+"wtext-delete-textblock.sh")
                  .arg("bilibili我关注的up主")
                  .arg("'<\/outline>'")
                  .arg(opmlPath)
                  );

    // 获取行号 bilibili我关注的up主
    int insertLineNumber = WTool::runCmdResultWithPipe("/bin/bash",QStringList()
                                <<"-c"
                                <<QString("grep -n bilibili我关注的up主 %1|cut -d ':' -f 1").arg(opmlPath)
                                ).toInt();

    QStringList lines = getFollowings(rssServer,"265657226");
    for (auto oneRecord: lines){
        //插入数据
        WTool::runCmd(QString("sed -i '%1a %2' %3")
                      .arg(insertLineNumber++)
                      .arg(oneRecord)
                      .arg(opmlPath));
    }
}

void RSSUpdateOPML::createFollowingsOPML(const QString & rssServer,const QString & vmid)
{
    //创建一个空白的opml文件
    QString content=
    "\""
    "<?xml version='1.0' encoding='utf-8'?>    \n"
    "<opml version='1.0'>                         \n"
    "<head>                                         \n"
    "<title>Feed Subscriptions</title>              \n"
    "</head>                                        \n"
    "<body>                                         \n"
    "</body>                                        \n"
    "</opml>                                        \n"
    "\""
            ;

    QString opmlPath=QString("~/Desktop/%2.opml")
            .arg(vmid+"_"+QDateTime::currentDateTime().toString("yyyy-MM-dd_hh"));
    qInfo()<<content;

    WTool::runCmd(QString("echo %1 > %2").arg(content).arg(opmlPath));

    //return;
    int insertLineNumber=6;
    QStringList lines = getFollowings(rssServer,vmid);
    for (auto oneRecord:lines){
        WTool::runCmd(QString("sed -i '%1a %2' %3")
                      .arg(insertLineNumber++)
                      .arg(oneRecord)
                      .arg(opmlPath));
    }
}

QStringList RSSUpdateOPML::getFollowings(const QString & rssServer,const QString &vmid)
{
    QString tmpfile = WConfig::ConfigDir + "/rss-bilibili.txt";
    QString cmd=QString("go run ~/wwork/1.bin/2.gotest/rss-bilibli.go %1 > %2")
            .arg(vmid).arg(tmpfile);
    WTool::runCmd(cmd);

    QStringList lines ;
    for (auto line:WTool::content(tmpfile)){
        line.replace("127.0.0.1:1200",rssServer);
        lines<<line;
    }
    WTool::runCmd("rm "+tmpfile);


    return lines;
}
