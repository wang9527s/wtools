#ifndef JSONCONFIG_H
#define JSONCONFIG_H

#include <QObject>

class WConfig
{
public:
    static QString OpacityCfgPath;
    static QString TimerTaskCfgPath;
    static QString TopTipCfgPath;
    static QString ConfigDir;
    static QString BashDir;
    static QString logPath;
    static void init();
};




#endif // JSONCONFIG_H
