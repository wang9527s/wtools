#ifndef RSSWIDGET_H
#define RSSWIDGET_H

#include <QWidget>

class QLineEdit;
class QPushButton;
class RssWidget : public QWidget
{
    Q_OBJECT
public:
    static RssWidget * instance();


private:
    explicit RssWidget(QWidget *parent = nullptr);
    ~RssWidget();

private slots:
    void createFollowingOPML();
    void updateBilibili();

private:
    QLineEdit * mRssService;
    QLineEdit * mBilibiliVmid;

    QPushButton * mCreateFollowingOpml;
    QPushButton * mUpdateOpml;
};

#endif // RSSWIDGET_H
