#ifndef WTOOL_H
#define WTOOL_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QSize>

class QTabWidget;
class WTool
{
public:
    // tabwidget样式
    static void customTab(QTabWidget *tabWidget);

    // 发送通知
    static void sendNotice(const QString &msg);
    static void showMsgdialogOnTopHint(const QString &title, const QString &msg);

    //是不是我的电脑
    static bool isAuthor();

    //允许bash
    static void runCmd(const QString cmd);
    static QString runCmdResultWithPipe(const QString &program, const QStringList &arguments);

    //读写json
    static QJsonObject getJsonFromConfig(const QString &pathname);
    static void saveJsonToConfig(QJsonObject &json, const QString &pathname);

    static QJsonObject stringToJson(QString string);
    static QString jsonToString(QJsonObject json);

    //读取普通文本文件
    static QStringList content(const QString &pathname);

    /*
        原本使用的是loadFromSvgRendrer 在缩放屏幕的时候 加载的svg图片有重影
        最好使用 loadPixmap
    */
    static QPixmap loadFromSvgRendrer(const QString &logoPath, QSize imgSize, double rendrerScale);
    static QPixmap loadFromQImageReader(const QString &logoPath, double rendrerScale);
    static QPixmap loadPixmap(const QString &file, QSize size = QSize());

    static bool appIsRuning(QString lockfile);
    static bool isDebug(int argc, char *argv[]);
};

#endif // WTOOL_H
