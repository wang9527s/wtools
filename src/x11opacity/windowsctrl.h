#ifndef WINDOWSCTRL_H
#define WINDOWSCTRL_H

#include <QMap>
#include <QObject>

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

    const QMap<int , QString> & wmInfo();
    void setOpacity(QList<int> winids,double opacity);
    void update();
    inline bool isChange(){return mChange;}

private:
    //key：winld      value : wmName
    QMap<int,QString> mMap;
    bool mChange;
};

#endif // WINDOWSCTRL_
