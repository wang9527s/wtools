#ifndef WINDOWSCTRL_H
#define WINDOWSCTRL_H

#include <QMap>
#include <QObject>
#include "src/tool/header.h"

struct WindowInfo {
    enum enum_status {
        Add,    // 新增为 标题为title的窗口
        Change, // 状态有变化，也许是标题相同的窗口增加了
        Remove, // 标题为 title的窗口没了
    } status;
    QString title;
    QList<int> wids;
};

class WindowsCtrl : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(WindowsCtrl)

public:
    static void setOpacity(QList<int> winids, double opacity);
    static QList<WindowInfo> winInfo();

private:
    static QMap<QString, QList<int>> getInfoFromProcess();
};

#endif // WINDOWSCTRL_
