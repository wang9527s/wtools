#include <QApplication>

#include "widget.h"
#include "src/tool/wconfig.h"
#include "src/tool/wtool.h"
#include "src/tool/log.h"
#include "src/rss/rssupdateopml.h"


#include <QDebug>
#include <QJsonObject>

#include "src/topTip/toptip.h"

#include "src/watermark/watermark.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);




    WConfig::init();
    logSysInit(WConfig::logPath+"wtool.log");
    qInfo()<<"run";

        TopTip::instance();
//        watermark water;

    QStringList args;
    for (int i=0;i<argc;++i) {
         args<<argv[i];
    }

    Widget w;
    if(args.contains("-show") || args.contains("--show")/* || args.contains("build-")*/)
        w.setVisible(true);



    return a.exec();
}
