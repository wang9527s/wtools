#ifndef APPMSGH_H
#define APPMSGH_H

#include <QObject>
#include <QPixmap>
#include <QFileInfo>

struct UpdateImageData {
    struct ImageInfo {
        QFileInfo fileinfo;
        QSize size;
    };
    ImageInfo image_info;

    // 缩放后的图片
    QImage img_square;

    // 和屏幕尺寸大小一致的背景图（存在多屏，所以用qlist）
    QList<QPixmap> bgs;

    // 原始图片
    QPixmap source_pix;
};

struct G_Data {
    int gl_square_side = 0;
};

class AppMsg : public QObject
{
    Q_OBJECT
public:
    bool exit = false;
    QList<QSize> screen_sizes;
    static AppMsg *instance()
    {
        static AppMsg obj;
        return &obj;
    }
signals:
    void sig_update_img(UpdateImageData);
    void sig_exit();
    void sig_show_next();

private:
    AppMsg(QObject *parent = nullptr)
        : QObject(parent)
    {
        qRegisterMetaType<UpdateImageData>("UpdateImageData");
    }
};

#endif // APPMSGH_H
