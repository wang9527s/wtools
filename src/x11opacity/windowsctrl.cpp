#include "windowsctrl.h"

#include <QProcess>
#include <QWindow>
#include <QDir>

#include <QDebug>
#include "src/tool/wtool.h"

/*
    TODO
    使用C++的MFC代码设置其它窗口的透明度，失败了；同样的函数python代码执行成功
    使用windowIgnore实在是太蠢了，看看能不能找到这些窗口的共同点
*/
QStringList windowIgnore =  QStringList()
        << "CDSPMetersMonitoring"
        << "GDI+ Window"
        << "WavesFXConnection_"
        << "AutoSubtypeOnAudioSession"
        << "BluetoothNotificationAreaIconWindowClass"
        << "DWM Notification "
        << "QTrayIconMessageWindow"
        << "MKSInvisibleWindow"
        << "OfficePowerManagerWindow"
        << "MS_WebcheckMonitor"
        << "UxdService"
        << "电池指示器"
        << "系统托盘溢出窗口"
        << "SessionDragWnd"
        << "位置通知"
        << "任务切换"
        << "Program Manager"
        << "Task Host Window"
           ;

void WindowsCtrl::setOpacity(QList<int> winids, double opacity)
{
    foreach (int winid, winids) {
#ifdef Plat_Windows
        QProcess process;
        QString app = WTool::AppDir().absoluteFilePath("Opacity.exe");
        QString cmd = QString("%1 -c 0 -s %2 %3")
                .arg(app)
                .arg(winid)
                .arg(int(opacity * 255));
        qInfo() << cmd;
        process.start(cmd);
        process.waitForFinished();
#endif
#ifdef Linux
        QWindow *p = QWindow::fromWinId((WId)winid);
        if (p) {
            p->setOpacity(opacity);
        }
#endif
    }
}

QList<WindowInfo> WindowsCtrl::winInfo()
{
    QMap<QString, QList<int>> wmInfo_old;
    QMap<QString, QList<int>> wmInfo;

    static QMap<QString, QList<int>> wmInfo_old_bak;
    wmInfo = getInfoFromProcess();
    wmInfo_old = wmInfo_old_bak;
    wmInfo_old_bak = wmInfo;

    QList<WindowInfo> res;
    WindowInfo wm;
    for (auto title: wmInfo.keys()){
        wm.title = title;
        wm.wids = wmInfo[title];
        if (wmInfo_old.contains(title)){
            if (wmInfo_old[title].size() == wmInfo[title].size()){
                // 删除相同
                wmInfo_old.remove(title);
                wmInfo.remove(title);
                continue;
            }
            else {
                // 有变化的窗口
                wm.status = WindowInfo::Change;
            }
        }
        else {
            // 新增的窗口
            wm.status = WindowInfo::Add;
        }
        res.append(wm);
    }

    wm.status = WindowInfo::Remove;
    auto itor = wmInfo_old.begin();
    while(itor != wmInfo_old.end()){
        wm.title = itor.key();
        wm.wids = itor.value();
        res.append(wm);
        ++itor;
    }

    return res;
}

QMap<QString, QList<int> > WindowsCtrl::getInfoFromProcess()
{
    QMap<QString, QList<int>> info;
    QProcess p;
#ifdef Plat_Windows
    QString app = WTool::AppDir().absoluteFilePath("Opacity.exe");
    p.start(QString("%1 -c 1 -g").arg(app));
    p.waitForFinished();
    QByteArray byte = p.readAllStandardOutput();
    QString s_res = QString::fromLocal8Bit(byte.data());
    QStringList lines = s_res.remove('\r').split('\n');
    lines.removeAll("");

    for (QString line : lines){
        QJsonObject json =  WTool::stringToJson(line);
        QJsonArray rect = json.value("rect").toArray();
        QString title = json.value("title").toString();
        int hd = json.value("hd").toInt();

        QRect rt(QPoint(rect[0].toInt(), rect[1].toInt()),
                QPoint(rect[2].toInt(), rect[3].toInt()));
        if (rt.width()<=1 || rt.height()<=1)
            continue;

        bool ignore = false;
        for (QString s_ignore: windowIgnore) {
            if (title.contains(s_ignore)) {
                ignore = true;
                break;
            }
        }
        if (ignore)
            continue;

        info[title].append(hd);
    }
#endif
#ifdef Linux
    p.start("wmctrl -p -G -l");
    p.waitForFinished();
    QByteArray byte = p.readAllStandardOutput();
    QStringList lines = QString(byte).split('\n');

    for (int i = 0; i < lines.size(); ++i) {
        int id = lines[i].left(10).toInt(nullptr, 16);
        QString name = lines[i].remove(0, 41);
        if (name == "")
            continue;

        name.remove("       N/A ");
        name.remove("N/A ");
        name.remove(QDir::homePath().split('/').last() + "-PC ");

        info[name].append(id);
    }
#endif
    return info;
}
