#include <QApplication>
#include <QSurfaceFormat>
#include <QScreen>
#include <QtConcurrent>

#include "desktop.h"
#include "windows_platform.h"
#include "func_header.h"
#include "AppMsg.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);

    Desktop_Wnds::instance()->init();

    for (auto *p : QGuiApplication::screens()) {
        // TODO
        // 见了鬼，如下代码，如果主屏上不创建desktop；则界面没有显示在正确的位置（桌面上方
        // 图标下方） if (p->geometry().topLeft() != QPoint(0, 0))
        //     continue;
        desktop *w = new desktop;
        w->resize(p->size());
        w->move(p->geometry().topLeft());
        w->setVisible(true);
        SetParent((HWND)w->winId(), Desktop_Wnds::instance()->progman);
        // w->lower();
        AppMsg::instance()->screen_sizes.append(p->size());
    }

    QtConcurrent::run([]() {
        while (!AppMsg::instance()->exit) {
            QString path =
                QString(":/img/img/h%1.JPG").arg(qrand() % 11 + 1, 2, 10, QLatin1Char('0'));
            QImage img(path);

            UpdateImageData data;
            int side = qMin(img.height(), img.width());
            data.img_square =
                img.copy(QRect((img.width() - side) / 2, (img.height() - side) / 2, side, side));
            data.img_square =
                data.img_square.scaled(500, 500, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
            for (QSize size : AppMsg::instance()->screen_sizes) {
                data.bgs.append(scaleBlurredPix(img, size));
            }

            emit AppMsg::instance()->sig_update_img(data);
            QThread::sleep(60);
        }
    });

    return app.exec();
}
