#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H

#include <QGraphicsView>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT

public:
    GraphicsView(QGraphicsScene *scene);

protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override{};
    void resizeEvent(QResizeEvent *event) override;
};

#endif // GRAPHICVIEW_H
