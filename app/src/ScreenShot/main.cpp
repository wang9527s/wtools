#include "screenshots.h"

#include <QApplication>
#include <QFontDialog>
#include <QDateTime>
#include <QScreen>
#include <QDebug>

#include "../utils/win/dump_helper.h"
#include "../utils/logger.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    logger_output_funcname = true;
    // logSysInit();
    // QString dir_path = QApplication::applicationDirPath() + "/dump";
    // dump_handle::start(dir_path, QApplication::applicationName());

    // ScreenShots  * w = new ScreenShots;
    // w->showRaise();

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

    // 创建一个足够大的 QPixmap 来保存所有屏幕的截图
    QPixmap pix(rt.size());
    QPainter painter(&pix);

    for (auto p : QGuiApplication::screens()) {
        QRect rect = p->geometry();

        // 抓取每个屏幕的截图
        QPixmap pixmap = p->grabWindow(0, 0, 0, rect.width(), rect.height());
        pixmap.setDevicePixelRatio(1);  // 强制截图的像素比为1，避免缩放引起的失真
        painter.drawPixmap(rect.topLeft(), pixmap);  // 将截图绘制到总的 QPixmap 上
    }

    // 保存截图
    pix.save("./555.png");

    // 创建 QLabel 并调整其大小以适应显示屏幕的总区域
    QLabel la;
    QSize logicalSize = rt.size() / pixelRatio;  // 将实际像素尺寸转换为逻辑尺寸
    la.resize(logicalSize);  // 使用逻辑尺寸来调整 QLabel 的大小

    // 设置 QLabel 的显示图片，确保图像缩放到正确的比例
    pix.setDevicePixelRatio(pixelRatio);

    la.setPixmap(pix);  // 设置 QPixmap 到 QLabel
    la.move(rt.topLeft());      // 将 QLabel 移动到屏幕左上角
    la.setVisible(true);  // 显示 QLabel

    return a.exec();
}
