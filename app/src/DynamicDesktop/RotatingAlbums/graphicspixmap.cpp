#include "graphicspixmap.h"

#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QCursor>
#include <QDebug>

GraphicsPixmap::GraphicsPixmap()
    : QGraphicsObject()
{
}

void GraphicsPixmap::setPixmap(const QPixmap &pixmap)
{
    pix = pixmap;
}

void GraphicsPixmap::setItemSize(QSize size)
{
    this->size = size;
}
void GraphicsPixmap::setItemPos(QPointF ponit)
{
    prepareGeometryChange();
    pos = ponit;
    update();
}
QPointF GraphicsPixmap::itemPos()
{
    return pos;
}
QSize GraphicsPixmap::itemSize()
{
    return size;
}

QRectF GraphicsPixmap::boundingRect() const
{
    return QRectF(pos, size);
};

void GraphicsPixmap::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setRenderHint(QPainter::SmoothPixmapTransform, true);
    QPixmap show_pix =
        pix.scaled(boundingRect().size().toSize(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    int draw_pos_x = pos.x() + (size.width() - show_pix.width()) / 2;
    int draw_pos_y = pos.y() + (size.height() - show_pix.height()) / 2;
    painter->drawPixmap(QPoint(draw_pos_x, draw_pos_y), show_pix);
    // painter->setPen(Qt::red);
    // painter->drawRect(boundingRect());
}
