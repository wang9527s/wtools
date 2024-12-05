#include "./setting/SettingWidget.h"

#include <QApplication>

#include "../utils/logger.hpp"
#include "../utils/win/dump_helper.h"

#include "../third/QHotkey/qhotkey.h"

#include "systemtray.h"
#include <QDebug>
#include <QSettings>

#include "Windows.h"

int main(int argc, char *argv[])
{
    // 设置当窗口关闭时不退出程序
    QApplication::setQuitOnLastWindowClosed(false);
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    logSysInit();

    QString dir_path = QApplication::applicationDirPath() + "/dump";
    dump_handle::start(dir_path, QApplication::applicationName());

    SystemTray w;

    // "Ctrl+Alt+Q"

    return a.exec();
}
