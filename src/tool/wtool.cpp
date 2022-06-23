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

void WTool::sendNotice(const QString &msg)
{
    //  notify-send
    system(QString("zenity --notification --text=\"%1\"").arg(msg).toStdString().c_str());
}

void WTool::showMsgdialogOnTopHint(const QString &title, const QString &msg)
{
    QMessageBox msgbox(QMessageBox::Information,title,msg);
    msgbox.setWindowFlag(Qt::WindowStaysOnTopHint);
    msgbox.setStyleSheet("QLabel{min-width: 120px;min-height:90px;}");
    msgbox.exec();
}

bool WTool::isAuthor()
{
    return QDir::homePath()=="/home/wangbin";
}

void WTool::runCmd(const QString cmd)
{
    qInfo()<<cmd;
    system(cmd.toStdString().c_str());
}

QString WTool::runCmdResultWithPipe(const QString &program, const QStringList &arguments )
{
    QProcess p;
    p.start(program,arguments, QIODevice::ReadOnly);
    p.waitForFinished();
    QString result=QString(p.readAllStandardOutput());
    qInfo()<<program<<arguments;
    qInfo()<<result;
    return result;
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

QStringList WTool::content(const QString &pathname)
{
    QFile file(pathname);
    QString strcontent;
    if (file.open(QIODevice::Text | QIODevice::ReadOnly))
    {
        QTextStream *out = new QTextStream(&file);
        out->setCodec("utf-8");
        strcontent = out->readAll();
        //strcontent = strcontent.simplified();
        file.close();
    }

    QStringList res = strcontent.split('\n');
    res.removeAll("");
    return res;
}

QPixmap WTool::getPixmapFromIcon(const QString &iconName)
{
    /* tip
       QIcon::fromTheme("/opt/apps/x11opacity-wang/window.svg")和QIcon::fromTheme("computer")都可以正确加载图标
       QIcon("/opt/apps/x11opacity-wang/window.svg")可以正确加载图标，QIcon("computer")加载图标失败
    */
    QPixmap pix;
    QIcon icon=QIcon::fromTheme(iconName);
    if (icon.isNull()){
        return pix;
    }

    const int s = 8;
    const int l[s] = { 64,16, 18, 24, 32, 96, 128, 256 };
    qInfo()<<pix.isNull();
    for (int i(0); i != s; ++i){
        pix=icon.pixmap(QSize(l[i],l[i]));
        qInfo()<<pix.isNull()<<l[i];
        if(!pix.isNull()){
            break;
        }
    }

    return pix;
}


