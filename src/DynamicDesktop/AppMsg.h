#ifndef APPMSGH_H
#define APPMSGH_H

#include <QObject>
#include <QPixmap>

struct UpdateImageData {
    QImage img_square;
    QList<QPixmap> bgs;
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

private:
    AppMsg(QObject *parent = nullptr)
        : QObject(parent)
    {
        qRegisterMetaType<UpdateImageData>("UpdateImageData");
    }
};

#endif // APPMSGH_H
