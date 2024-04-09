#ifndef DESKTOP_H
#define DESKTOP_H

#include <QWidget>

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

private:
    QLabel *plabel;
    GLWidget *gl;
    RotatingAlbums *album;
};

#endif // DESKTOP_H
