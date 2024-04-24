#include <QApplication>
#include <QSurfaceFormat>
#include <QScreen>
#include <QtConcurrent>

#include "desktop.h"
#include "windows_platform.h"
#include "func_header.h"
#include "AppMsg.h"
#include <QDebug>
#include "ImagePathnameManager.h"
#include "../utils/win/dump_helper.h"
#include "../utils/logger.hpp"

int main(int argc, char *argv[])
{
    // atexit(exit_func);

    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    logger_output_funcname = true;
    logSysInit();
    QString dir_path = QApplication::applicationDirPath() + "/dump";
    dump_handle::start(dir_path, QApplication::applicationName());

    QString rcc = QApplication::applicationDirPath() + "/rcc/bgs.rcc";
    QResource::registerResource(rcc);
    ImagePathnameManager::instance()->init_rcc();

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Hooker::run();

    for (auto *p : QGuiApplication::screens()) {
        // TODO
        // 见了鬼，如下代码，如果主屏上不创建desktop；则界面没有显示在正确的位置（桌面上方
        // 图标下方） if (p->geometry().topLeft() != QPoint(0, 0))
        //     continue;
        desktop *w = new desktop;
        w->resize(p->size());
        w->move(p->geometry().topLeft());
        w->setVisible(true);
        SetParent((HWND)w->winId(), Hooker::winid_progman);
        // w->lower();
        AppMsg::instance()->screen_sizes.append(p->size());
    }

    QFuture<void> future = QtConcurrent::run([]() {
        while (true) {
            QString path = g_random_bg_path();
            QImage img(path);
            if (img.isNull())
                continue;

            UpdateImageData data;
            int side = qMin(img.height(), img.width());
            data.img_square =
                img.copy(QRect((img.width() - side) / 2, (img.height() - side) / 2, side, side));
            data.img_square =
                data.img_square.scaled(500, 500, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            for (QSize size : AppMsg::instance()->screen_sizes) {
                data.bgs.append(scaleBlurredPix(img, size));
            }
            data.source_pix = QPixmap::fromImage(img);

            emit AppMsg::instance()->sig_update_img(data);

            for (int i = 0; i < 10; i++) {
                QThread::sleep(6);
                if (AppMsg::instance()->exit) {
                    return;
                }
            }
        }
    });

    QObject::connect(AppMsg::instance(), &AppMsg::sig_exit, [&] {
        Hooker::exit();
        future.waitForFinished();
        qInfo() << "exit app";
        app.exit();
    });

    return app.exec();
}
