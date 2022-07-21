#ifndef WINDOWSCTRL_H
#define WINDOWSCTRL_H

#include <QMap>
#include <QObject>
#include "src/tool/header.h"

class WindowsCtrl : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(WindowsCtrl)

public:
    const QMap<int, QString> &wmInfo();
    void setOpacity(QList<int> winids, double opacity);
    void update();
    inline bool isChange()
    {
        return mChange;
    }

private:
    // key：winld      value : wmName
    QMap<int, QString> mMap;
    bool mChange;
};

#endif // WINDOWSCTRL_
