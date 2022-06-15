#ifndef UI_H
#define UI_H

#include <QWidget>
#include <QMap>
#include <QWindow>




class QListWidget;
class QSlider;
class opacityWidget : public QWidget
{
    Q_OBJECT

public:
    static opacityWidget * instance(){
        static opacityWidget * pIns=nullptr;
        if (pIns==nullptr) {
            pIns=new opacityWidget;
        }
        return pIns;
    }
protected:
    explicit opacityWidget(QWidget *parent = nullptr);

    void timerEvent(QTimerEvent * e);
    void updateList();
private:
    void updateOpacity();

private slots:
    void updateChecked();
private:


    QMap<QString,bool> mCheckeds;           //QCheckBox的勾选状态
    QMap<QString,double> mOpacitys;         //窗口名所对应的透明度

    QSlider * mSlider;
    QListWidget * mlistWidget;

};

#endif // UI_H
