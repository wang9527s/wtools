#include <QApplication>
#include "src/tool/wconfig.h"

#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QStringList args;
    for (int i=0;i<argc;++i) {
         args<<argv[i];
    }
    WConfig::init();

    Widget w;
    w.setFixedSize(600,400);
    if(args.contains("-show") || args.contains("--show")/* || args.contains("build-")*/)
        w.setVisible(true);

    return a.exec();
}
