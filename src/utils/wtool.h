#ifndef WTOOL_H
#define WTOOL_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>

class WTool
{
public:
    // 发送通知
    static void sendNotice(const QString &msg);
    static void showMsgdialogOnTopHint(const QString &title, const QString &msg);

    // 允许bash
    static void runCmd(const QString cmd);
    static QString runCmdResultWithPipe(const QString &program, const QStringList &arguments);

    // 读写json
    static QJsonObject getJsonFromConfig(const QString &pathname);
    static void saveJsonToConfig(QJsonObject &json, const QString &pathname);

    static QJsonObject stringToJson(QString string);
    static QString jsonToString(QJsonObject json);

    static bool appIsRuning(QString lockfile);
    static bool isDebug(int argc, char *argv[]);
};

#endif // WTOOL_H
