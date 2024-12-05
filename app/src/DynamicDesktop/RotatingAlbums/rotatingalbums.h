#ifndef ROTATINGALBUMS_H
#define ROTATINGALBUMS_H

#include <QWidget>

#include <QWidget>
#include <QMap>

#include "graphicspixmap.h"

class QTimer;
class QPropertyAnimation;
class GraphicsPixmap;
class QGraphicsScene;
class QParallelAnimationGroup;
class RotatingAlbums : public QWidget
{
    Q_OBJECT

public:
    RotatingAlbums(QWidget *parent = 0);
    const int album_height = 120;
public slots:
    void show_next();

protected:
    void updatePropertySize(int w, int h);
    void resizeEvent(QResizeEvent *evt);

private:
    void play();
    void updateAnimation();

private:
    QGraphicsScene *m_scene;
    QList<ItemProperty> m_itemInfos;
    QList<GraphicsPixmap *> m_items;
    QParallelAnimationGroup *m_group;
    QMap<GraphicsPixmap *, QPropertyAnimation *> m_animas_pos;
    QMap<GraphicsPixmap *, QPropertyAnimation *> m_animas_size;
    QTimer *m_timer;
};

#endif // ROTATINGALBUMS_H
