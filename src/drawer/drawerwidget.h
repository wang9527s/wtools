#ifndef DRAWERWIDGET_H
#define DRAWERWIDGET_H

#include <QWidget>
#include "src/tool/header.h"

class QLineEdit;
class QLabel;
class DrawerWidget : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(DrawerWidget)

protected:
    explicit DrawerWidget(QWidget *parent = nullptr);

protected slots:
    void onShowIcon();
    void onShowSvg();

private:
    QLineEdit *mName;
    QLineEdit *mSvgPath;

    QLabel *m_showlabel = nullptr;
};

#endif // DRAWERWIDGET_H
