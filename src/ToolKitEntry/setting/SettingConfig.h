#ifndef SETTINGCONFIG_H
#define SETTINGCONFIG_H

#include <QJsonObject>
#include <QKeySequence>
#include "../utils/wtool.h"
#include <QCoreApplication>

struct SettingInfo {
    struct Album {
        bool enable;
        QString dir;
    };
    struct ScreenShot {
        bool enable;
        QKeySequence hotkey;
    };

    Album album;
    ScreenShot screen_shot;
    bool autoStart;

    void init(QJsonObject json)
    {
        autoStart = json.value("autoStart").toBool(false);
        QJsonObject album_js = json.value("album").toObject();
        album.dir = album_js.value("dir").toString("");
        album.enable = album_js.value("enable").toBool(true);

        QJsonObject screen_shot_js = json.value("screen_shot").toObject();
        screen_shot.hotkey = QKeySequence(screen_shot_js.value("hotkey").toString("F2"));
        screen_shot.enable = screen_shot_js.value("enable").toBool(true);
    }
    QJsonObject value()
    {
        QJsonObject album_js;
        album_js.insert("dir", album.dir);
        album_js.insert("enable", album.enable);
        QJsonObject screen_shot_js;
        screen_shot_js.insert("hotkey", screen_shot.hotkey.toString());
        screen_shot_js.insert("enable", screen_shot.enable);

        QJsonObject json;
        json.insert("autoStart", autoStart);
        json.insert("album", album_js);
        json.insert("screen_shot", screen_shot_js);
        return json;
    }
};

class SettingConfig
{

public:
    SettingInfo d;
    SettingConfig()
    {
        QString cfg_path = qApp->applicationDirPath() + "/config";
        QDir dir;
        dir.mkpath(cfg_path);

        pathname = cfg_path + "/config.json";
        qInfo() << pathname;
        d.init(WTool::getJsonFromConfig(pathname));
    }
    void save()
    {
        auto json = d.value();
        WTool::saveJsonToConfig(json, pathname);
    }

private:
    QString pathname;
};

#endif // SETTINGCONFIG_H
