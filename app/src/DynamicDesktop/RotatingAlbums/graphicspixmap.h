#ifndef GRAPHICSPIXMAP_H
#define GRAPHICSPIXMAP_H

#include <QObject>
#include <QGraphicsObject>
#include <QPixmap>
#include <QRectF>

struct ItemProperty {
    QRectF rt;
    float opacity;
    int zIndex;
};
class GraphicsPixmap : public QGraphicsObject
{
    Q_OBJECT

    // 动画会调用属性接口，更改属性
    Q_PROPERTY(QPointF item_pos READ itemPos WRITE setItemPos)
    Q_PROPERTY(QSize item_size READ itemSize WRITE setItemSize)

public:
    GraphicsPixmap();
    void setPixmap(const QPixmap &pixmap);

public:
    void setItemSize(QSize size);
    void setItemPos(QPointF ponit);
    QPointF itemPos();
    QSize itemSize();

    QRectF boundingRect() const override;
    void paint(QPainter *, const QStyleOptionGraphicsItem *, QWidget *) override;

private:
    QPixmap pix;
    QPointF pos;
    QSize size;
};

#endif // GRAPHICSPIXMAP_H
