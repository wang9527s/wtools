#include "systemtray.h"

#include "./setting/SettingWidget.h"
#include "../third/QHotkey/qhotkey.h"
#include "CallExternal.h"
#include <QDebug>
#include <QCoreApplication>
#include <QMenu>
#include <QProcess>

SystemTray::SystemTray(QObject *parent)
    : QObject(parent)
{
    pIcon = new QSystemTrayIcon(QIcon("://img/app.ico"));
    pIcon->setVisible(true);

    set = new SettingWidget;
    menu = new QMenu;
    menu->addAction(u8"显示设置");
    menu->addAction(u8"退出程序");
    set->setVisible(true);

    connect(menu, &QMenu::triggered, [=](QAction *action) {
        if (action->text() == u8"显示设置") {
            set->setVisible(true);
        }
        else if (action->text() == u8"退出程序") {
            qApp->exit();
        }
    });

    connect(pIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::Trigger) {
            qDebug() << "Left button clicked";
            set->setVisible(true);
        }
        else if (reason == QSystemTrayIcon::Context) {
            qDebug() << "Right button clicked";
            menu->exec(QCursor::pos());
        }
    });

    hotkey = new QHotkey(set->cfg->d.screen_shot.hotkey, true, this);
    connect(hotkey, &QHotkey::activated, qApp, [&]() {
        if (set->cfg->d.screen_shot.enable)
            CallExternal::instance()->startScreenShot();
    });
}

SystemTray::~SystemTray()
{
    CallExternal::instance()->exitAlbum();
    delete pIcon;
    delete menu;
    delete set;
}
