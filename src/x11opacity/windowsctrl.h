#ifndef WINDOWSCTRL_H
#define WINDOWSCTRL_H

#include <QMap>
#include <QObject>
#include <QProcess>
#include <QWindow>
#include <QDir>

class WindowsCtrl: public QObject
{
    Q_OBJECT

public:
    static WindowsCtrl * instance(){
        static WindowsCtrl * pIns=nullptr;
        if(pIns == nullptr){
            pIns=new WindowsCtrl;
        }
        return pIns;
    }


    const QMap<int , QString> & wmInfo(){
        return mMap;
    }

    void setOpacity(QList<int> winids,double opacity){
        foreach(int id,winids){
            setOpacity(id,opacity);
        }
    }
    void setOpacity(int winid,double opacity){
            QWindow * p = QWindow::fromWinId((WId)winid);
            if (p){
                p->setOpacity(opacity);
            }
    }

    void update(){
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
    bool isChange(){return mChange;}

private:
    //key：winld      value : wmName
    QMap<int,QString> mMap;
    bool mChange;
};

#endif // WINDOWSCTRL_
