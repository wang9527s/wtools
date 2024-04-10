#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

#include <QMessageLogger>
#include <qlogging.h>
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QFileInfo>
#include <QCoreApplication>
#include <QDir>

QFile *logger_file = NULL;

const char *logger_msg_type[] = {"Debug   ", "Warning ", "Critical", "Fatal   ", "Info    "};
bool logger_output_funcname = false;
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString current_date_time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz");

    QString msgText;
    if (logger_output_funcname) {
        QString file = QFileInfo(context.file).fileName();
        QString func = QString(context.function).split("(").first().split(" ").last();
        QString postion = QString("%1:%2, %3")
                              .arg(file.mid(0, 19), -20) // 不足15 右侧补充空格
                              .arg(context.line, 4, 10)
                              .arg(func.mid(0, 24), 25);
        msgText = QString("[%1] %4 | %2 | %3\n")
                      .arg(logger_msg_type[type])
                      .arg(postion)
                      .arg(msg)
                      .arg(current_date_time);
    }
    else {
        msgText =
            QString("[%1] %2 | %3\n").arg(logger_msg_type[type]).arg(current_date_time).arg(msg);
    }
    if (logger_file) {
        logger_file->write(msgText.toUtf8());
        logger_file->flush();
    }

    QByteArray localMsg = msgText.toLocal8Bit();
    fprintf(stdout, "%s", localMsg.constData());
    fflush(stdout);
}

void logSysInit(QString filePath = "")
{
    if (filePath == "") {
        QString path = qApp->applicationDirPath() + "\\log\\";
        QString name = qApp->applicationName() + ".log";
        filePath = path + name;
        QDir dir;
        dir.mkpath(path);
    }
    logger_file = new QFile(filePath);
    if (!logger_file->open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append)) {
        return;
    }
    // 初始化自定义日志处理函数myMessageOutput
    qInstallMessageHandler(myMessageOutput);
}

#endif // LOGGER_H
