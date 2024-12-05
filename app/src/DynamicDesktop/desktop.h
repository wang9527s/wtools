#ifndef DESKTOP_H
#define DESKTOP_H

#include <QWidget>
#include "AppMsg.h"

class QLabel;
class GLWidget;
class RotatingAlbums;
class desktop : public QWidget
{
    Q_OBJECT

public:
    desktop(QWidget *parent = nullptr);
    ~desktop() {}
    void resizeEvent(QResizeEvent *evt) override;

public slots:
    void update_gl_label(UpdateImageData data);

private:
    QLabel *plabel;
    GLWidget *gl;
    RotatingAlbums *album;
};

#endif // DESKTOP_H
