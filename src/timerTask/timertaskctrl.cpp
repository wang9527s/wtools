#include "timertaskctrl.h"

#include <QMap>
#include <QString>
#include <QTimer>

#include "src/tool/wconfig.h"
#include "src/tool/wtool.h"

bool TimerTaskCtrl::appendTask(TimerTaskInfo &task)
{
    if (mDatas.contains(task)) {
        return false;
    }

    mDatas.append(task);
    saveDataToConfig();

    return true;
}

void TimerTaskCtrl::saveDataToConfig()
{
    QJsonArray data;
    for (auto e : mDatas) {
        data.append(e.toJson());
    }

    QJsonObject json;
    json.insert("data", data);

    WTool::saveJsonToConfig(json, WConfig::timerTaskCfgPath());
}

void TimerTaskCtrl::startTasks()
{
    for (auto e : mDatas) {
        if (e.hintTask()) {
            WTool::showMsgdialogOnTopHint("定时任务", e.recordText());
        }
    }
}

void TimerTaskCtrl::loopCheckTaskInAMinutes()
{
    QTimer::singleShot(60 * 1000, this, &TimerTaskCtrl::loopCheckTaskInAMinutes);
    startTasks();
}

TimerTaskCtrl::TimerTaskCtrl(QObject *parent)
    : QObject(parent)
{
    // showTask
    QJsonObject json = WTool::getJsonFromConfig(WConfig::timerTaskCfgPath());
    for (auto e : json.value("data").toArray()) {
        QJsonObject oneRecord = e.toObject();
        TimerTaskInfo task(oneRecord);

        mDatas.append(task);
    }

    loopCheckTaskInAMinutes();
}
