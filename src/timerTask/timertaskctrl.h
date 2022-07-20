#ifndef TIMERTASKCTRL_H
#define TIMERTASKCTRL_H

#include <QObject>
#include <QJsonObject>


#include "timertaskinfo.h"

class TimerTaskCtrl:public QObject{
    Q_OBJECT

public:
    static TimerTaskCtrl * instance(){
        static TimerTaskCtrl * pIns=nullptr;
        if(pIns==nullptr){
            pIns=new TimerTaskCtrl(nullptr);
        }
        return pIns;
    }
    const QList<TimerTaskInfo> & data(){return mDatas;}
    bool appendTask(TimerTaskInfo & task);
private:

    void saveDataToConfig();
    void startTasks();
public slots:
    //一个小时检查一次任务
    void loopCheckTaskInAMinutes();

private:
    TimerTaskCtrl(QObject*parent);
    QList<TimerTaskInfo> mDatas;
};

#endif // TIMERTASKCTRL_H
