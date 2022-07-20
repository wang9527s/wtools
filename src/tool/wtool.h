#ifndef WTOOL_H
#define WTOOL_H

#include <QObject>

#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

class QTabWidget;
class WTool
{
public:
    //tabwidget样式
    static void customTab(QTabWidget * tabWidget);

    // 发送通知
    static void sendNotice(const QString & msg);
    static void showMsgdialogOnTopHint(const QString & title,const QString & msg);

    //是不是我的电脑
    static bool isAuthor();

    //允许bash
    static void runCmd(const QString cmd);
    static QString runCmdResultWithPipe(const QString &program, const QStringList &arguments);

    //读写json
    static QJsonObject getJsonFromConfig(const QString &pathname);
    static void saveJsonToConfig(QJsonObject & json, const QString & pathname);

    //读取普通文本文件
    static QStringList content(const QString & pathname);
};

#endif // WTOOL_H
