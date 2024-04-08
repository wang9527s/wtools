#ifndef FUNC_HEADER_H
#define FUNC_HEADER_H

#include <QImage>
#include <QPixmap>

extern QImage blurEffect(QImage in_img, QRect rt);
extern QPixmap scaleBlurredPix(QImage in_img, QSize out_size);

#endif // FUNC_HEADER_H
