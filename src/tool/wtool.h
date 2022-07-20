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

    //读写json
    static QJsonObject getJsonFromConfig(const QString &pathname);
    static void saveJsonToConfig(QJsonObject & json, const QString & pathname);
};

#endif // WTOOL_H
