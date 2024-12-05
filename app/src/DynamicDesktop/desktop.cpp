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
#include "watermarks.h"

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
    QString title = data.image_info.fileinfo.fileName();
    QString content = QString(u8"尺寸：%1 x %2\n%3")
                          .arg(data.image_info.size.width())
                          .arg(data.image_info.size.height())
                          .arg(data.image_info.fileinfo.filePath());
    WaterMarks::instance()->setText(title, content);
    WaterMarks::instance()->setVisible(true);
}
