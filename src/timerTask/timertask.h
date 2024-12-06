#ifndef TIMERTASK_H
#define TIMERTASK_H

#include <QWidget>
#include <QMap>
#include <QString>
#include <QJsonObject>
#include "src/tool/header.h"

namespace Ui
{
class TimerTask;
}

class TimerTaskInfo;
class TimerTask : public QWidget
{
    Q_OBJECT
    DECLARE_SINGLETON(TimerTask)

private:
    explicit TimerTask(QWidget *parent = nullptr);
    ~TimerTask();

    bool getInputTask(TimerTaskInfo &task);
    void addTask(const TimerTaskInfo &task);
    // 插入一条task
    void appendTaskToWidget(TimerTaskInfo task);

private:
    void initDefaultTask();
private slots:
    void on_weekCbx_textChanged(const QString &text);
    void on_taskType_textChanged(const QString &text);
    void on_create_clicked();

private:
    Ui::TimerTask *ui;
};

#endif // TIMERTASK_H
