#include <QApplication>

#include "src/tool/wconfig.h"
#include "src/tool/log.h"
#include "src/topTip/toptip.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList args;
    for (int i=0;i<argc;++i) {
         args<<argv[i];
    }
    WConfig::init();
    logSysInit(WConfig::logPath+"wtool.log");

    TopTip::instance();

    Widget w;
    w.setFixedSize(600,400);
    if(args.contains("-show") || args.contains("--show")/* || args.contains("build-")*/)
        w.setVisible(true);

    return a.exec();
}
