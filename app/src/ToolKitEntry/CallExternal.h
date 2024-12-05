#ifndef CALLEXTERNAL_H
#define CALLEXTERNAL_H

#include "../utils/instance_header.h"
#include "../utils/wtool.h"
#include <QDebug>
#include <QString>
#include <QProcess>
#include <QCoreApplication>

class CallExternal
{
    DECLARE_SINGLETON(CallExternal)

    void startAlbum()
    {
        if (album_process.state() == QProcess::NotRunning) {
            QString cmd_album = qApp->applicationDirPath() + "/app_DynamicDesktop.exe";
            album_process.start(cmd_album);
        }
    }
    void startScreenShot()
    {
        QString cmd_ss = qApp->applicationDirPath() + "/app_ScreenShot.exe";
        // cmd_ss = QDir::toNativeSeparators(cmd_ss);
        qInfo().noquote() << "start screen shot";
        WTool::runCmd(cmd_ss);
    }
    void exitAlbum()
    {
        album_process.close();
        // waitForFinished没啥作用
        album_process.waitForFinished();
    }

private:
    QProcess album_process;
};

#endif // CALLEXTERNAL_H
