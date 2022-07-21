#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>
#include "src/tool/header.h"

class QLineEdit;
class IconWidget : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(IconWidget)

protected:
    explicit IconWidget(QWidget *parent = nullptr);

protected slots:
    void onShowIcon();

private:
    QLineEdit *mName;
};

#endif // WIDGET_H
