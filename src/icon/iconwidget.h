#ifndef ICONWIDGET_H
#define ICONWIDGET_H

#include <QWidget>

namespace Ui {
    class IconWidget;
}

class IconWidget : public QWidget
{
    Q_OBJECT

public:
    static IconWidget * instance(){
        static IconWidget * pIns=nullptr;
        if(pIns==nullptr){
            pIns=new IconWidget;
        }
        return  pIns;
    }
protected:
    explicit IconWidget(QWidget *parent = nullptr);
    ~IconWidget();

public slots:
    void on_show_clicked();
private:
    Ui::IconWidget *ui;
};

#endif // WIDGET_H
