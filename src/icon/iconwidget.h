#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>
#include "src/tool/header.h"

class QLineEdit;
class QLabel;
class IconWidget : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(IconWidget)

protected:
    explicit IconWidget(QWidget *parent = nullptr);

protected slots:
    void onShowIcon();
    void onShowSvg();

private:
    QLineEdit *mName;
    QLineEdit *mSvgPath;

    QLabel *m_showlabel = nullptr;
};

#endif // WIDGET_H
