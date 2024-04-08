#include "windows_platform.h"

#include <QDebug>

bool Desktop_Wnds::init()
{
    progman = FindWindow(L"Progman", NULL);
    // 发送一个消息给Progman窗口，以便它创建WorkerW窗口
    SendMessageTimeout(progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

    // 找到WorkerW类的窗口
    hWorkerW = FindWindowEx(NULL, NULL, L"WorkerW", NULL);
    // qInfo() << "hWorkerW:" <<hWorkerW;
    // 通过遍历找到包含SHELLDLL_DefView的WorkerW
    while ((!hDefView) && hWorkerW) {
        hDefView = FindWindowEx(hWorkerW, NULL, L"SHELLDLL_DefView", NULL);
        hWorkerW = FindWindowEx(NULL, hWorkerW, L"WorkerW", NULL);

        // qInfo() << "hDefView:" <<hDefView;
        // qInfo() << "hWorkerW:" <<hWorkerW;
    }

    // 隐藏窗口，不让mainwindow被挡住
    ShowWindow(hWorkerW, 0);

    hFloderView = FindWindowEx(hDefView, NULL, L"SysListView32", nullptr);
    qInfo() << hDefView << hWorkerW << hFloderView;
    return true;
}
