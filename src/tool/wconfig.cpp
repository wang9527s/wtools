#include "wconfig.h"

#include <QDir>

 QString WConfig::OpacityCfgPath;
 QString WConfig::TimerTaskCfgPath;
 QString WConfig::TopTipCfgPath;
 QString WConfig::ConfigDir;
 QString WConfig::BashDir;
  QString WConfig::logPath;

void WConfig::init()
{
    QDir dir;
    QString path(QDir::homePath()+"/.config/wtools/");
    dir.mkpath(path);
    dir.cd(path);

    ConfigDir = dir.absolutePath();
    OpacityCfgPath=ConfigDir+ "/opacity.json";
    TimerTaskCfgPath=ConfigDir + "/Timertask.json";
    TopTipCfgPath=ConfigDir+ "/toptip.json";
    BashDir=QDir::homePath()+"/wwork/1.bin/1.bash/";


    logPath=QDir::homePath()+"/.cache/wtools/";
    dir.mkpath(logPath);
}



