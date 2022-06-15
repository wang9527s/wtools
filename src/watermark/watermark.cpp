#include "watermark.h"

#include "src/tool/wtool.h"
#include <QDebug>
#include <QWindow>

watermark::watermark(QWidget *parent) : QWidget(parent)
{
    QStringList res = WTool::runCmdResultWithPipe("/bin/bash",QStringList()
                                       <<"-c"
                                       <<"wmctrl -pGl | grep -E '桌面|Desktop'"
                                       ).split('\n');
    res.removeAll("");

    // 多屏的话，选取最后一个
    QStringList desktopInfo = res.last().split(' ');
    desktopInfo.removeAll("");

    // 桌面的winid
    QString wid = desktopInfo[0];
    QRect desktopRect(QPoint(desktopInfo[3].toInt(),desktopInfo[4].toInt()),
            QSize(desktopInfo[5].toInt(),desktopInfo[6].toInt()));




    qInfo()<<"this"<<this->winId()<<this<<QWidget::find(this->winId())->geometry()<<QWindow::fromWinId(this->winId())->geometry();

    QWindow * desktop = QWindow::fromWinId(wid.toInt(nullptr,16));
    qInfo()<<"desktop"<<QWidget::find(wid.toInt(nullptr,16));

    setFixedSize(300,200);
    move(1920,0);
    setVisible(true);
}
