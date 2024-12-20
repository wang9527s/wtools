#ifndef TIMERTASKINFO_H
#define TIMERTASKINFO_H

#include <QObject>
#include <QDateTime>
#include <QString>

extern QMap<int, QString> dayOfWeekMap;
extern QMap<int, QString> taskTypeMap;

class QJsonObject;
class TimerTaskInfo //: public QObject
{

public:
    int type;        // 1 每周任务        2 每天任务      3 一次性定时任务
    qint64 datetime; // type==3时候是datetime   type==1或2的时候是time
    QString tip;

    int dayOfWeek = 0;
    // 赋值
    TimerTaskInfo &operator=(const TimerTaskInfo &info);
    TimerTaskInfo(QJsonObject &js);
    TimerTaskInfo(){};
    // 判等
    bool operator==(const TimerTaskInfo &info) const;

    // 功能函数
    QJsonObject toJson();
    QString recordText();

    // 命中任务(任务时间到)
    bool hintTask();

private:
    // 任务本次开启时间
    QDateTime hiteTaskCurTime();
};

#endif // TIMERTASKINFO_H
