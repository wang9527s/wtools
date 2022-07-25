#include "wtool.h"

#include <QJsonDocument>
#include <QJsonParseError>

#include <QTabWidget>
#include <QTabBar>
#include "customtabstyle.h"

#include <QMessageBox>
#include <QDir>
#include <QProcess>
#include <QDebug>

#include <QImageReader>
#include <QSvgRenderer>
#include <QApplication>

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
    QMessageBox msgbox(QMessageBox::Information, title, msg);
    msgbox.setWindowFlag(Qt::WindowStaysOnTopHint);
    msgbox.setStyleSheet("QLabel{min-width: 120px;min-height:90px;}");
    msgbox.exec();
}

bool WTool::isAuthor()
{
    return QDir::homePath() == "/home/wangbin";
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

QStringList WTool::content(const QString &pathname)
{
    QFile file(pathname);
    QString strcontent;
    if (file.open(QIODevice::Text | QIODevice::ReadOnly)) {
        QTextStream *out = new QTextStream(&file);
        out->setCodec("utf-8");
        strcontent = out->readAll();
        // strcontent = strcontent.simplified();
        file.close();
    }

    QStringList res = strcontent.split('\n');
    res.removeAll("");
    return res;
}

QPixmap WTool::loadFromSvgRendrer(const QString &logoPath, QSize imgSize, double rendrerScale)
{
    QSvgRenderer svgRender;
    svgRender.load(logoPath);
    QPixmap pix(imgSize);
    pix.fill(Qt::transparent);
    QPainter p(&pix);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing |
                     QPainter::SmoothPixmapTransform);
    svgRender.render(&p);
    return pix;
}

QPixmap WTool::loadFromQImageReader(const QString &logoPath, double rendrerScale)
{
    QImageReader render;
    render.setFileName(logoPath);
    render.setScaledSize(render.size() * rendrerScale);
    QPixmap pix = QPixmap::fromImage(render.read());
    pix.setDevicePixelRatio(rendrerScale);
    return pix;
}

QPixmap WTool::loadPixmap(const QString &file, QSize size)
{
    qreal ratio = 1.0;
    qreal devicePixel = qApp->devicePixelRatio();

    QPixmap pixmap;

    if (!qFuzzyCompare(ratio, devicePixel) || size.isValid()) {
        QImageReader reader;
        //        reader.setFileName(qt_findAtNxFile(file, devicePixel, &ratio));
        reader.setFileName(file);
        if (reader.canRead()) {
            reader.setScaledSize(
                (size.isNull() ? reader.size() : reader.size().scaled(size, Qt::KeepAspectRatio)) *
                (devicePixel / ratio));
            pixmap = QPixmap::fromImage(reader.read());
            pixmap.setDevicePixelRatio(devicePixel);
        }
    }
    else {
        pixmap.load(file);
    }

    return pixmap;
}
