#include "windowsctrl.h"

#include <QProcess>
#include <QWindow>
#include <QDir>

const QMap<int, QString> &WindowsCtrl::wmInfo()
{
    return mMap;
}

void WindowsCtrl::setOpacity(QList<int> winids, double opacity)
{
    foreach(int winid,winids){
        QWindow * p = QWindow::fromWinId((WId)winid);
        if (p){
            p->setOpacity(opacity);
        }
    }
}

void WindowsCtrl::update()
{
    QProcess p;
    p.start("wmctrl -p -G -l");
    p.waitForFinished();
    QByteArray byte = p.readAllStandardOutput();
    QStringList lines = QString(byte).split('\n');
    static QStringList perlines;
    if(perlines==lines){
        mChange=false;
        return;
    }

    //窗口有所增减
    mChange=true;
    perlines=lines;

    mMap.clear();
    for(int i=0;i<lines.size();++i){
        int id = lines[i].left(10).toInt(nullptr,16);
        QString name = lines[i].remove(0,41);
        if (name == "")
            continue;

        name.remove("       N/A ");
        name.remove("N/A ");
        name.remove(QDir::homePath().split('/').last()+"-PC ");
        mMap.insert(id,name);
    }
}
