#include "timertask.h"
#include "ui_timertask.h"

#include <QDebug>
#include "src/tool/wconfig.h"
#include "src/tool/wtool.h"
#include "timertaskctrl.h"
#include "timertaskinfo.h"

TimerTask::TimerTask(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::TimerTask)
{
    ui->setupUi(this);

    for (auto item : dayOfWeekMap.toStdMap()) {
        ui->tasktypeCbx->addItem(item.second, item.first);
    }
    for (auto item : taskTypeMap.toStdMap()) {
        ui->weekCbx->addItem(item.second, item.first);
    }

    connect(
        ui->tasktypeCbx, &QComboBox::currentTextChanged, this, &TimerTask::on_taskType_textChanged);
    connect(ui->weekCbx, &QComboBox::currentTextChanged, this, &TimerTask::on_weekCbx_textChanged);
    on_taskType_textChanged("每周任务");

    // showTask
    for (auto task : TimerTaskCtrl::instance()->data()) {
        appendTaskToWidget(task);
    }

    ui->dateEdit->setDate(QDate::currentDate());
    ui->timeEdit->setTime(QTime::currentTime());

    initDefaultTask();
}

TimerTask::~TimerTask()
{
    delete ui;
}

bool TimerTask::getInputTask(TimerTaskInfo &task)
{
    const QString taskmsg = ui->taskmsg->text();
    if (taskmsg.isEmpty()) {
        WTool::sendNotice("请输入任务内容");
        return false;
    }

    task.tip = ui->taskmsg->text();
    task.type = ui->tasktypeCbx->currentData()
                    .toInt(); // 1 每周任务        2 每天任务      3 一次性定时任务
    task.datetime = QDateTime(ui->dateEdit->date(), ui->timeEdit->time()).toSecsSinceEpoch();
    switch (task.type) {
    case 1:
        task.dayOfWeek = ui->tasktypeCbx->currentData().toInt();
        break;
    case 2:
        break;
    case 3:
        break;
    default:
        WTool::sendNotice("任务类型出错");
        return false;
        break;
    }

    return true;
}

void TimerTask::appendTaskToWidget(TimerTaskInfo task)
{
    QString text = task.recordText();
    ui->textEdit->append(text);
}

void TimerTask::initDefaultTask()
{
    if (!WTool::isAuthor()) {
        return;
    }

    TimerTaskInfo task;
    task.type = 1;

    int dayofweek[] = {1, 2, 3, 4, 5};
    for (auto i : dayofweek) {
        task.tip = i == 4 ? "周报" : "日报";
        task.datetime = QDateTime(QDate(), QTime(i == 4 ? 16 : 17, 0)).toSecsSinceEpoch();
        task.dayOfWeek = i;

        // TODO
        // addTask(task);
    }
}

void TimerTask::on_weekCbx_textChanged(const QString &text)
{
    qInfo() << ui->weekCbx->currentData(Qt::UserRole) << ui->weekCbx->currentText();
}

void TimerTask::on_taskType_textChanged(const QString &text)
{
    qInfo() << ui->tasktypeCbx->currentData(Qt::UserRole) << ui->tasktypeCbx->currentText();
    ui->timeEdit->setVisible(true);
    ui->dateEdit->setVisible(false);
    ui->weekCbx->setVisible(false);

    // 1 每周任务        2 每天任务      3 一次性定时任务
    switch (ui->tasktypeCbx->currentData(Qt::UserRole).toInt()) {
    case 1:
        ui->weekCbx->setVisible(true);
        break;
    case 2:
        break;
    case 3:
        ui->dateEdit->setVisible(true);
        break;
    default:
        WTool::sendNotice("任务类型出错");
        break;
    }
}

void TimerTask::on_create_clicked()
{
    TimerTaskInfo task;
    if (!getInputTask(task))
        return;

    if (!TimerTaskCtrl::instance()->appendTask(task))
        return;

    appendTaskToWidget(task);
}
