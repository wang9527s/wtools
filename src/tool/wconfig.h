#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <QObject>

class WConfig
{
public:
    static QString opacityCfgPath();
    static QString timerTaskCfgPath();
    static QString topTipCfgPath();
    static QString ConfigDir;
    //    static QString BashDir;
    static QString logPath();
    static void init();
};

#endif // JSONCONFIG_H
