#include "wconfig.h"

#include <QDir>

QString WConfig::ConfigDir;

QString WConfig::opacityCfgPath()
{
    return ConfigDir + "/opacity.json";
}

QString WConfig::timerTaskCfgPath()
{
    return ConfigDir + "/Timertask.json";
}

QString WConfig::topTipCfgPath()
{
    return ConfigDir + "/toptip.json";
}

QString WConfig::logPath()
{
    return QDir::homePath() + "/.cache/wtools/";
}

void WConfig::init()
{
    QDir dir;
    QString path(QDir::homePath() + "/.config/wtools/");
    dir.mkpath(path);
    dir.cd(path);

    ConfigDir = dir.absolutePath();

    dir.mkpath(logPath());
}
