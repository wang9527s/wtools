#ifndef TopTip_H
#define TopTip_H

#include <QFrame>
#include "src/tool/header.h"

#include <QColor>
struct TopTipBlock {
    QRectF rt;
    QColor tlColor;
    QColor brColor;
    QString text;


    bool operator == (const TopTipBlock & e) const {
        return e.rt == rt && e.text == text
                && e.tlColor == tlColor && e.brColor == brColor;
    }

};

enum TopTipPosition{
    TopRight,
    ButtonRight,
};

class TopTip : public QFrame
{
    DECLARE_SINGLETON(TopTip)

protected:
    void paintEvent(QPaintEvent * evt);
    void mouseReleaseEvent(QMouseEvent * evt);
private:
    QString getButtleText();
    qreal hToPointX(int h,int min = 0);
    void initBlock();
    void readConfig();
    TopTip(QWidget * parent=nullptr);

    QList<TopTipBlock> mBlocks;
    QTimer*  mShowTimer;
    TopTipPosition mPosition = TopRight;
};

#endif // TopTip_H
