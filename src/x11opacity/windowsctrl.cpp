#include "windowsctrl.h"

#include <QProcess>
#include <QWindow>
#include <QDir>



void WindowsCtrl::setOpacity(QList<int> winids, double opacity)
{
    foreach (int winid, winids) {
        QWindow *p = QWindow::fromWinId((WId)winid);
        if (p) {
            p->setOpacity(opacity);
        }
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
    return info;
}
