#include "func_header.h"

#include <QImage>
#include <QPainter>
#include <QDebug>

// 模糊 参数1（图像）矩形区域外的部分
QImage blurEffect(QImage in_img, QRect rt, QSize out_size)
{
    const int radius = 5;
    if (in_img.width() < out_size.width() || in_img.height() < out_size.height()) {
        in_img = in_img.scaled(out_size, Qt::KeepAspectRatioByExpanding, Qt::FastTransformation);
    }
    QImage resultImage(out_size, QImage::Format_ARGB32);
    resultImage.fill(Qt::transparent);

    QPainter painter(&resultImage);
    // 对每个像素应用均值模糊
    for (int y = 0; y < in_img.height() && y < out_size.height(); ++y) {
        for (int x = 0; x < in_img.width() && x < out_size.width(); ++x) {
            if (rt.contains(x, y))
                continue;

            int red = 0, green = 0, blue = 0, alpha = 0;
            int pixelCount = 0;

            // 对当前像素周围的像素应用均值模糊
            for (int dy = -radius; dy <= radius; ++dy) {
                for (int dx = -radius; dx <= radius; ++dx) {
                    int nx = x + dx;
                    int ny = y + dy;

                    if (nx >= 0 && nx < in_img.width() && ny >= 0 && ny < in_img.height()) {
                        QRgb pixel = in_img.pixel(nx, ny);

                        red += qRed(pixel);
                        green += qGreen(pixel);
                        blue += qBlue(pixel);
                        alpha += qAlpha(pixel);

                        ++pixelCount;
                    }
                }
            }

            // 计算均值
            red /= pixelCount;
            green /= pixelCount;
            blue /= pixelCount;
            alpha /= pixelCount;

            // 将均值后的像素写入结果图像
            QRgb blurredPixel = qRgba(red, green, blue, alpha);
            resultImage.setPixel(x, y, blurredPixel);
        }
    }

    return resultImage;
}

QPixmap scaleBlurredPix(QImage in_img, QSize out_size)
{
    QImage show_content = in_img.scaled(out_size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QPoint blurred_tl((out_size.width() - show_content.width()) / 2,
                      (out_size.height() - show_content.height()) / 2);
    QRect blurred_rt(blurred_tl, show_content.size());
    // qInfo() << "show_content.size()" << show_content.size() << "out_size" << out_size
    //         << "blurred_tl" << blurred_tl << "blurred_rt" << blurred_rt;
    QPixmap blurredPixmap = QPixmap::fromImage(blurEffect(in_img, blurred_rt, out_size));

    QPixmap pix(out_size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.drawPixmap(0, 0, blurredPixmap);
    painter.drawPixmap(blurred_tl, QPixmap::fromImage(show_content));

    return pix;
}
