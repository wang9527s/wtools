#include "screenshots.h"

#include <QApplication>
#include <QFontDialog>
#include <QDateTime>

#include "../utils/win/dump_helper.h"
#include "../utils/logger.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    logger_output_funcname = true;
    logSysInit();
    QString dir_path = QApplication::applicationDirPath() + "/dump";
    dump_handle::start(dir_path, QApplication::applicationName());

    ScreenShots w;
    w.showRaise();
    return a.exec();
}
