#include <QApplication>

#include "src/tool/wconfig.h"
#include "src/tool/log.h"
#include "src/topTip/toptip.h"
#include "src/tool/wtool.h"
#include "widget.h"

#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (WTool::appIsRuning(a.applicationName())){
        qInfo()<<"项目正在运行中";
        return -1;
    }

    WConfig::init();
    logSysInit(WConfig::logPath() + "wtool.log");

    TopTip::instance();
    Widget w;
    w.setFixedSize(760, 480);
    w.setVisible(WTool::isDebug(argc,argv));

    return a.exec();
}
