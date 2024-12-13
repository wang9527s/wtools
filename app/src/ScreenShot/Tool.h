#ifndef TOOL_H
#define TOOL_H

#include <QApplication>
#include <QPixmap>
#include <QRect>
#include <QScreen>
#include <QPainter>
#include <QClipboard>

class Tool
{
public:
    static QPixmap copy(const QPixmap & pix, QRect rt){
        qreal k = pix.devicePixelRatio();
        return pix.copy(QRect(rt.topLeft() * k, rt.size() * k));
    }

    // 返回两个顶点组成的矩形
    static QRect rt(QPoint p1, QPoint p2)
    {
        int x = qMin(p1.x(), p2.x());
        int y = qMin(p1.y(), p2.y());
        int w = qAbs(p1.x() - p2.x());
        int h = qAbs(p1.y() - p2.y());
        return QRect(x, y, w, h);
    };

    static void copyPixmapToClipboard(QPixmap pixmap)
    {
        QImage image = pixmap.toImage();
        // 获取剪贴板对象
        QClipboard *clipboard = QGuiApplication::clipboard();
        clipboard->setImage(image);
    }
};

#endif // TOOL_H
