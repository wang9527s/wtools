#include "rotatingalbums.h"

#include "RotatingAlbums.h"
#include "graphicsview.h"
#include "graphicspixmap.h"
#include "ImagePathnameManager.h"
#include "AppMsg.h"

#include <QParallelAnimationGroup>
#include <QPropertyAnimation>
#include <QDebug>
#include <QTimer>

#include <QHBoxLayout>
#include <QGraphicsScene>

RotatingAlbums::RotatingAlbums(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(album_height);
    m_scene = new QGraphicsScene(this);
    // 视图
    GraphicsView *view = new GraphicsView(m_scene);
    view->setFrameShape(QFrame::NoFrame);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->setBackgroundBrush(QColor(46, 46, 46));
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QHBoxLayout *pl = new QHBoxLayout(this);
    pl->setMargin(0);
    // pl->setContentsMargins(500, 0,500,0);
    pl->addWidget(view);

    // 动画: 大小，位置
    m_group = new QParallelAnimationGroup;

    // 样式表
    setObjectName("RotatingAlbums");
    setAttribute(Qt::WA_StyledBackground);
    setStyleSheet("QGraphicsView{background:transparent;}"
                  "QWidget#RotatingAlbums{ background-color: rgba(240, 240, 240, 150); "
                  "border-radius:25px;border: 1px solid #dcdcdc;}");

    m_timer = new QTimer(this);
    m_timer->setInterval(2000);
    connect(m_timer, &QTimer::timeout, [this]() { play(); });
    connect(m_group, &QParallelAnimationGroup::finished, [&]() { m_timer->start(); });
    m_timer->start();

    connect(AppMsg::instance(), &AppMsg::sig_update_img, this, [=](UpdateImageData data) {
        m_items[m_items.size() / 2]->setPixmap(data.source_pix);
    });
}

void RotatingAlbums::show_next()
{
    play();
}

void RotatingAlbums::updatePropertySize(int w, int h)
{
    if (m_items.size() > 0)
        return;

    int item_w = 160;
    const int item_h = album_height - 20;
    const int spacing = 10;
    int count = w / (item_w + spacing);
    if ((count + 0.5f) * item_w < w) {
        // 动态调整一下item的宽度
        count++;
        item_w = w / count - spacing;
    }

    for (int i = 0; i < count; i++) {
        ItemProperty property;
        int zindex = count - i > i ? i + 1 : count - i;

        property.zIndex = zindex;
        float k = 1.0 - (count / 2 + 1 - zindex) * 0.05f;

        property.opacity = k;

        int x = i * (item_w + spacing);
        int y = 10;
        int t_item_h = item_h * k;
        y += (item_h - t_item_h) / 2;
        property.rt = QRect(x, y, item_w, t_item_h);
        m_itemInfos.append(property);
    }

    for (int i = 0; i < m_itemInfos.size(); i++) {
        auto property = m_itemInfos[i];
        QPixmap pixmap(ImagePathnameManager::instance()->pathname(i));
        GraphicsPixmap *item = new GraphicsPixmap();
        item->setPixmap(pixmap);
        m_items << item;
        m_scene->addItem(item);

        item->setItemSize(property.rt.size().toSize());
        item->setItemPos(property.rt.topLeft());
        item->setZValue(property.zIndex);
        item->setOpacity(property.opacity);

        auto a_pos = new QPropertyAnimation(item, "item_pos");
        auto a_size = new QPropertyAnimation(item, "item_size");
        m_animas_pos.insert(item, a_pos);
        m_animas_size.insert(item, a_size);
        a_pos->setDuration(1000);
        a_size->setDuration(1000);
        a_pos->setEasingCurve(QEasingCurve::OutQuad);
        a_size->setEasingCurve(QEasingCurve::OutQuad);
        m_group->addAnimation(a_pos);
        m_group->addAnimation(a_size);
    }
}

void RotatingAlbums::resizeEvent(QResizeEvent *evt)
{
    m_scene->setSceneRect(QRect(0, 0, width(), height()));
    updatePropertySize(width(), height());
    return QWidget::resizeEvent(evt);
}

void RotatingAlbums::updateAnimation()
{
    for (int index = 0; index < m_itemInfos.size(); index++) {
        const auto item = m_items[index];
        auto a_pos = m_animas_pos.value(item);
        auto a_size = m_animas_size.value(item);
        ItemProperty info = m_itemInfos[index];
        item->setZValue(info.zIndex);
        item->setOpacity(info.opacity);

        a_pos->setStartValue(item->itemPos());
        a_size->setStartValue(item->itemSize());
        a_pos->setEndValue(info.rt.topLeft());
        a_size->setEndValue(info.rt.size().toSize());
    }
}

void RotatingAlbums::play()
{
    m_group->stop();
    GraphicsPixmap *firstItem = m_items.takeAt(0);
    m_items << firstItem;
    updateAnimation();
    m_group->start();
}
