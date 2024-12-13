#ifndef GRAB_DESKTOP_H
#define GRAB_DESKTOP_H

#include <QRect>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QPainter>

class GrabDesktop {
public:
    QPixmap bg;
    void grab() {
        qreal pixelRatio = 1;  // 默认缩放比例为1
        QRect rt(0, 0, 0, 0);

        // 获取所有屏幕的信息
        // TODO 主屏？存在负坐标
        for (auto pScreen : QGuiApplication::screens()) {
            QRect srt;

            // 判断屏幕是否包含原点 (0, 0)，并获取对应的缩放比例
            if (pScreen->geometry().contains(QPoint(0, 0))) {
                pixelRatio = pScreen->devicePixelRatio();  // 获取包含(0, 0)的屏幕的缩放比例
            }

            // 更新总的截图区域，考虑所有屏幕的几何信息
            srt.setTopLeft(pScreen->geometry().topLeft());
            srt.setSize(pScreen->geometry().size() * pScreen->devicePixelRatio());
            rt = rt.united(srt);
        }

        qInfo() << "Total area rect:" << rt;

        bg = QPixmap(rt.size());
        QPainter painter(&bg);

        for (auto p : QGuiApplication::screens()) {
            QRect rect = p->geometry();

            // 抓取每个屏幕的截图
            QPixmap pixmap = p->grabWindow(0);
            pixmap.setDevicePixelRatio(1);  // 强制截图的像素比为1，避免缩放引起的失真
            painter.drawPixmap(rect.topLeft(), pixmap);  // 将截图绘制到总的 QPixmap 上
        }
        bg.setDevicePixelRatio(pixelRatio);
    }
};


#endif // GRAB_DESKTOP_H
