#include "graphicsview.h"

GraphicsView::GraphicsView(QGraphicsScene *scene)
    : QGraphicsView(scene)
{
}

void GraphicsView::resizeEvent(QResizeEvent *event)
{
    QGraphicsView::resizeEvent(event);
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}
