#include "desktop.h"

#include <Windows.h>
#include "glwidget.h"
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#include <QPainter>
#include <QGraphicsBlurEffect>
#include <QtConcurrent>
#include "AppMsg.h"
#include "RotatingAlbums/rotatingalbums.h"

desktop::desktop(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);

    gl = new GLWidget(this);
    album = new RotatingAlbums(this);

    plabel = new QLabel(this);
    plabel->setAlignment(Qt::AlignCenter);
    plabel->lower();
    plabel->setStyleSheet("background:yellow;");
    connect(AppMsg::instance(), &AppMsg::sig_update_img, this, &desktop::update_gl_label);
    connect(AppMsg::instance(), &AppMsg::sig_show_next, album, &RotatingAlbums::show_next);
}

void desktop::resizeEvent(QResizeEvent *evt)
{
    gl->setFixedSize(300, 300);
    plabel->resize(size());
    gl->move(width() - gl->width(), 0);
    album->setFixedWidth(width() * 7 / 10);
    album->move((width() - album->width()) / 2, height() - album->height() - 60);
    return QWidget::resizeEvent(evt);
}

void desktop::update_gl_label(UpdateImageData data)
{
    for (QPixmap pix : data.bgs) {
        if (pix.size() == size()) {
            plabel->setPixmap(pix);
            break;
        }
    }
    gl->updateTextures(data.img_square);
}
