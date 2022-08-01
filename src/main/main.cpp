#include <QApplication>

#include "src/tool/wconfig.h"
#include "src/tool/log.h"
#include "src/topTip/toptip.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    WConfig::init();
    logSysInit(WConfig::logPath() + "wtool.log");

    TopTip::instance();
    Widget w;
    w.setFixedSize(760, 480);

    QStringList args;
    for (int i = 0; i < argc; ++i) {
        args << argv[i];
    }
    w.setVisible(!args.contains("-r"));

    return a.exec();
}
