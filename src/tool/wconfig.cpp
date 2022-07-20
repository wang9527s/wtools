#include "wconfig.h"

#include <QDir>

 QString WConfig::OpacityCfgPath;
 QString WConfig::ConfigDir;


void WConfig::init()
{
    QDir dir;
    QString path(QDir::homePath()+"/.config/wtools/");
    dir.mkpath(path);
    dir.cd(path);

    ConfigDir = dir.absolutePath();
    OpacityCfgPath=ConfigDir+ "/opacity.json";
}



