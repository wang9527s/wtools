#include "wtool.h"

#include <QTabWidget>
#include <QTabBar>
#include "customtabstyle.h"

#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>

void WTool::customTab(QTabWidget *tabWidget)
{
    tabWidget->setTabPosition(QTabWidget::West);
    tabWidget->tabBar()->setStyle(new CustomTabStyle);
}

QJsonObject WTool::getJsonFromConfig(const QString &pathname)
{
    QFile f(pathname);
    QMap<QString,double> res;
    if (!f.open(QIODevice::ReadOnly))
        return QJsonObject();

    QJsonParseError error;
    QJsonDocument doc=QJsonDocument::fromJson(f.readAll(),&error);
    if(doc.isNull() || error.error != QJsonParseError::NoError)
        return QJsonObject();

    return doc.object();
}

void WTool::saveJsonToConfig(QJsonObject &json, const QString &pathname)
{
    QFile f(pathname);
    if (!f.open(QIODevice::WriteOnly))
        return;

    QJsonDocument doc;
    doc.setObject(json);
    f.write(doc.toJson());
    f.close();
}
