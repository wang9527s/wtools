#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QMap>
#include <QWindow>
#include "src/tool/header.h"

class QListWidget;
class QSlider;
class QListWidgetItem;
class opacityWidget : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(opacityWidget)

protected:
    explicit opacityWidget(QWidget *parent = nullptr);
    QListWidgetItem * item(QString text);
    void timerEvent(QTimerEvent *e);

private slots:
    void onSliderChanged();

private:
    QMap<QString, bool> mCheckeds;   // QCheckBox的勾选状态
    QMap<QString, double> mOpacitys; //窗口名所对应的透明度

    QSlider *mSlider;
    QListWidget *mlistWidget;
};

#endif // UI_H
