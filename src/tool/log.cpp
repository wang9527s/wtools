#include "log.h"

#include <QMessageLogger>
#include <qlogging.h>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfo>

QFile *gFileLog = NULL;

char *msgHead[] = {"Debug   ", "Warning ", "Critical", "Fatal   ", "Info    "};

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");
    QString file = QFileInfo(context.file).fileName();
    QString func = QString(context.function).split("(").first().split(" ").last();
    QString postion = QString("%1:%2, %3")
            .arg(file.mid(0,19), -20)                          // 不足15 右侧补充空格
            .arg(context.line ,4, 10)
            .arg(func.mid(0,24),25)
            ;
    QString msgText = QString("[%1] %4 | %2 | %3\n")
            .arg(msgHead[type])
            .arg(postion)
            .arg(msg)
            .arg(current_date_time);
    if (gFileLog) {
        gFileLog->write(msgText.toUtf8());
        gFileLog->flush();
    }

    QByteArray localMsg = msgText.toLocal8Bit();
    fprintf(stdout,"%s",localMsg.constData());
    fflush(stdout);
}

void logSysInit(QString filePath)
{
    gFileLog = new QFile(filePath);
    if (!gFileLog->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }
    //初始化自定义日志处理函数myMessageOutput
    qInstallMessageHandler(myMessageOutput);
}
