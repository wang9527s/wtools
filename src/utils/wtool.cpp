#include "wtool.h"

#include <QJsonDocument>
#include <QJsonParseError>

#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>
#include <QImageReader>
#include <QApplication>

#include <QLockFile>
#include <QSettings>

void WTool::sendNotice(const QString &msg)
{
#ifdef Plat_Linux
    //  notify-send "你好哇" --app-name='dde-control-center' --icon='preferences-system'notify-send
    system(QString("zenity --notification --text=\"%1\"").arg(msg).toStdString().c_str());
#endif
#ifdef Plat_Windows
    QMessageBox::information(nullptr, QStringLiteral("提示"), msg);
#endif
}

void WTool::showMsgdialogOnTopHint(const QString &title, const QString &msg)
{
    QMessageBox msgbox(QMessageBox::Information, title, msg);
    msgbox.setWindowFlag(Qt::WindowStaysOnTopHint);
    msgbox.setStyleSheet("QLabel{min-width: 120px;min-height:90px;}");
    msgbox.exec();
}

void WTool::runCmd(const QString cmd)
{
    qInfo() << cmd;
    system(cmd.toStdString().c_str());
}

QString WTool::runCmdResultWithPipe(const QString &program, const QStringList &arguments)
{
    QProcess p;
    p.start(program, arguments, QIODevice::ReadOnly);
    p.waitForFinished();
    QString result = QString(p.readAllStandardOutput());
    qInfo() << program << arguments;
    qInfo() << result;
    return result;
}

QJsonObject WTool::getJsonFromConfig(const QString &pathname)
{
    QFile f(pathname);
    QMap<QString, double> res;
    if (!f.open(QIODevice::ReadOnly))
        return QJsonObject();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(f.readAll(), &error);
    if (doc.isNull() || error.error != QJsonParseError::NoError)
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

QJsonObject WTool::stringToJson(QString string)
{
    QJsonDocument json = QJsonDocument::fromJson(string.toLocal8Bit().data());
    if (json.isNull()) {
        qDebug() << "error";
    }
    return json.object();
}

QString WTool::jsonToString(QJsonObject json)
{
    return QString(QJsonDocument(json).toJson());
}

bool WTool::appIsRuning(QString lockfile)
{
    QString path = QDir::temp().absoluteFilePath(lockfile);
    QLockFile lockFile(path);
    // 上锁失败，认为app在后台运行
    return !lockFile.tryLock(100);
}

bool WTool::isDebug(int argc, char *argv[])
{
    // 目前参数列表中只有-r
    QStringList args;
    for (int i = 0; i < argc; ++i) {
        args << argv[i];
    }
    return !args.contains("-r");
}

QString auto_start_key("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run");
void WTool::setAutoStart(bool enable)
{
#ifdef Plat_Windows
    QSettings settings(auto_start_key, QSettings::NativeFormat);

    QString appName = QApplication::applicationName();
    if (enable) {
        QString appPath = QApplication::applicationFilePath();
        appPath = QDir::toNativeSeparators(appPath);
        settings.setValue(appName, appPath);
    }
    else {
        settings.remove(appName);
    }
#endif
}

bool WTool::isAutoStart()
{
#ifdef Plat_Windows
    QSettings settings(auto_start_key, QSettings::NativeFormat);
    return settings.contains(QApplication::applicationName());
#endif
}
