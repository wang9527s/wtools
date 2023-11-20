#include "timertaskinfo.h"

#include <QJsonObject>
#include <QMap>

QMap<int, QString> dayOfWeekMap = {{1, u8"每周任务"}, {2, u8"每天任务"}, {3, u8"定时任务"}};
QMap<int, QString> taskTypeMap = {{1, u8"周一"},
                                  {2, u8"周二"},
                                  {3, u8"周三"},
                                  {4, u8"周四"},
                                  {5, u8"周五"},
                                  {6, u8"周六"},
                                  {7, u8"周日"}};

TimerTaskInfo &TimerTaskInfo::operator=(const TimerTaskInfo &info)
{
    type = info.type;
    datetime = info.datetime;
    tip = info.tip;
    dayOfWeek = info.dayOfWeek;

    return *this;
}

TimerTaskInfo::TimerTaskInfo(QJsonObject &js)
{
    this->type = js.value("type").toInt();
    this->datetime = js.value("datetime").toInt();
    this->tip = js.value("tip").toString();
    this->dayOfWeek = js.value("dayOfWeek").toInt();
}

bool TimerTaskInfo::operator==(const TimerTaskInfo &info)
{
    return type == info.type && datetime == info.datetime && tip == info.tip &&
           dayOfWeek == info.dayOfWeek;
}

QJsonObject TimerTaskInfo::toJson()
{
    QJsonObject js;
    js.insert("type", this->type);
    js.insert("datetime", this->datetime);
    js.insert("tip", this->tip);
    js.insert("dayOfWeek", this->dayOfWeek);
    return js;
}

QDateTime TimerTaskInfo::hiteTaskCurTime()
{
    QDate date = QDateTime::fromSecsSinceEpoch(datetime).date();
    QTime time = QDateTime::fromSecsSinceEpoch(datetime).time();
    switch (type) {
    case 1:
        // line1=QString("每周任务：%1
        // %2").arg(taskTypeMap.value(task.dayOfWeek)).arg(datetime.time().toString("hh:mm"));
        // TODO
        break;
    case 2:
        // 每天任务
        date = QDate::currentDate();
        break;
    case 3:
        break;
    default:
        break;
    }
    return QDateTime(date, time);
}

QString TimerTaskInfo::recordText()
{
    QString line1;
    // 1 每周任务        2 每天任务      3 一次性定时任务
    switch (type) {
    case 1:
        line1 = QString("每周任务：%1 %2")
                    .arg(taskTypeMap.value(dayOfWeek))
                    .arg(QDateTime::fromSecsSinceEpoch(datetime).time().toString("hh:mm"));
        break;
    case 2:
        line1 = QString("每天任务：%1")
                    .arg(QDateTime::fromSecsSinceEpoch(datetime).time().toString("hh:mm"));
        break;
    case 3:
        line1 = QString("一次性定时任务：%1")
                    .arg(QDateTime::fromSecsSinceEpoch(datetime).toString("yyyy-MM-dd hh:mm"));
        break;
    default:
        // qInfo()<<"任务类型出错";
        system("zenity --notification --text=\"任务类型出错\"");
        break;
    }

    return QString(line1 + '\n' + "任务提示：" + tip + '\n');
}

bool TimerTaskInfo::hintTask()
{
    // TODO  和hiteTaskNextTime一起 用于判断临界条件
    return qAbs(hiteTaskCurTime().toSecsSinceEpoch() - QDateTime::currentSecsSinceEpoch()) <= 40;
}
