#ifndef TIMERTASKCTRL_H
#define TIMERTASKCTRL_H

#include <QObject>

#include "timertaskinfo.h"
#include "src/tool/header.h"

class TimerTaskCtrl : public QObject
{
    Q_OBJECT
    DECLARE_SINGLETON(TimerTaskCtrl)

public:
    const QList<TimerTaskInfo> &data()
    {
        return mDatas;
    }
    bool appendTask(TimerTaskInfo &task);

private:
    void saveDataToConfig();
    void startTasks();
public slots:
    // 一个小时检查一次任务
    void loopCheckTaskInAMinutes();

private:
    TimerTaskCtrl(QObject *parent = nullptr);
    QList<TimerTaskInfo> mDatas;
};

#endif // TIMERTASKCTRL_H
