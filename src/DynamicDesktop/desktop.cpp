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

desktop::desktop(QWidget *parent)
    : QWidget(parent)
{
    setWindowFlag(Qt::FramelessWindowHint);

    GLWidget *gl = new GLWidget(this);
    QHBoxLayout *pl = new QHBoxLayout(this);
    pl->addWidget(gl);

    plabel = new QLabel(this);
    plabel->setAlignment(Qt::AlignCenter);
    plabel->lower();
    plabel->setStyleSheet("background:yellow;");
    connect(
        AppMsg::instance(),
        &AppMsg::sig_update_img,
        this,
        [=](UpdateImageData data) {
            for (QPixmap pix : data.bgs) {
                if (pix.size() == size()) {
                    plabel->setPixmap(pix);
                    break;
                }
            }
            gl->updateTextures(data.img_square);
        },
        Qt::QueuedConnection);
}

void desktop::resizeEvent(QResizeEvent *evt)
{
    plabel->resize(size());
    return QWidget::resizeEvent(evt);
}
