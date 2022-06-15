#ifndef RSSWIDGET_H
#define RSSWIDGET_H

#include <QWidget>

namespace Ui {
class RssWidget;
}

class RssWidget : public QWidget
{
    Q_OBJECT
public:
    static RssWidget * instance(){
        static RssWidget * pIns =nullptr;
        if(pIns==nullptr){
            pIns=new RssWidget;
        }
        return  pIns;
    }

private:
    explicit RssWidget(QWidget *parent = nullptr);
    ~RssWidget();

private slots:
    void on_createFollowingOPML_clicked();
    void on_updateBilibili_clicked();

private:
    Ui::RssWidget * ui;
};

#endif // RSSWIDGET_H
