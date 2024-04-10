#include "windows_platform.h"

#include <QDebug>
#include <QSet>
#include <Thread>
#include "AppMsg.h"

namespace Hooker
{
HWND winid_progman = nullptr;
HWND winid_FloderView = nullptr;
HWND winid_workerw = nullptr;
HWND winid_DefView = nullptr;

HHOOK hook_key = nullptr;
HHOOK hook_mouse = nullptr;
HWINEVENTHOOK hook_event = nullptr;

#define Exit_Hook_Thread (WM_USER + 1)
std::thread hookThread;

QSet<HWND> winids;
DWORD thread_id;

bool focus_in_desktop = false;

void print_window(QString tip, HWND hWnd)
{
    WCHAR winClass[128];
    WCHAR winTitle[128];
    GetClassName(hWnd, winClass, 128);
    GetWindowText(hWnd, winTitle, 128);
    qInfo() << tip << hWnd << "winTitle:" << QString::fromWCharArray(winTitle)
            << ", winClass:" << QString::fromWCharArray(winClass);
}

// 全局键盘钩子的回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *kbStruct = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        bool ctrl_pressed = GetAsyncKeyState(VK_CONTROL) & 0x8000;
        if (kbStruct->vkCode == VK_ESCAPE && ctrl_pressed) {
            // Hooker::exit();
            AppMsg::instance()->exit = true;
            emit AppMsg::instance()->sig_exit();
        }
        if (focus_in_desktop) {
            qInfo() << kbStruct->vkCode << VK_RIGHT;
            if (kbStruct->vkCode == VK_RIGHT && ctrl_pressed) {
                emit AppMsg::instance()->sig_show_next();
            }
        }
    }

    // 将消息传递给下一个钩子
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// 全局鼠标钩子的回调函数
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0 && wParam == WM_LBUTTONDOWN) {
        // 提取鼠标事件信息
        MSLLHOOKSTRUCT *pMouseHook = reinterpret_cast<MSLLHOOKSTRUCT *>(lParam);
        HWND click_wid = WindowFromPoint(pMouseHook->pt);
        if (Hooker::winid_FloderView == click_wid) {
            // qInfo() << u8"yes";
            focus_in_desktop = true;
        }
    }

    // 将消息传递给下一个钩子
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

void CALLBACK WinEventProc(HWINEVENTHOOK hWinEventHook,
                           DWORD event,
                           HWND hwnd,
                           LONG idObject,
                           LONG idChild,
                           DWORD dwEventThread,
                           DWORD dwmsEventTime)
{
    if (event == EVENT_SYSTEM_FOREGROUND) {
        // 窗口激活事件
        HWND wid = GetForegroundWindow();
        if (!Hooker::winids.contains(wid)) {
            // qInfo() << "leave";
            focus_in_desktop = false;
        }

        // print_window("GetForegroundWindow()", GetForegroundWindow());
    }
}

void startHook()
{
    qInfo() << "register hook";
    hook_key = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    hook_mouse = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
    hook_event = SetWinEventHook(EVENT_SYSTEM_FOREGROUND,
                                 EVENT_SYSTEM_FOREGROUND,
                                 NULL,
                                 WinEventProc,
                                 0,
                                 0,
                                 WINEVENT_OUTOFCONTEXT);
}

void stopHook()
{
    qInfo() << "unregister hook";
    if (hook_key != NULL) {
        UnhookWindowsHookEx(hook_key);
        hook_key = NULL;
    }

    if (hook_mouse != NULL) {
        UnhookWindowsHookEx(hook_mouse);
        hook_mouse = NULL;
    }

    if (hook_event != NULL) {
        UnhookWinEvent(hook_event);
        hook_event = NULL;
    }
}

bool initDesktopHwnd()
{
    winid_progman = FindWindow(L"Progman", NULL);
    // 发送一个消息给Progman窗口，以便它创建WorkerW窗口
    SendMessageTimeout(winid_progman, 0x052C, 0, 0, SMTO_NORMAL, 1000, nullptr);

    // 找到WorkerW类的窗口
    winid_workerw = FindWindowEx(NULL, NULL, L"WorkerW", NULL);
    // qInfo() << "hWorkerW:" <<hWorkerW;
    // 通过遍历找到包含SHELLDLL_DefView的WorkerW
    while ((!winid_DefView) && winid_workerw) {
        winid_DefView = FindWindowEx(winid_workerw, NULL, L"SHELLDLL_DefView", NULL);
        winid_workerw = FindWindowEx(NULL, winid_workerw, L"WorkerW", NULL);

        qInfo() << "winid_DefView:" << winid_DefView << "winid_workerw:" << winid_workerw;
        winids.insert(winid_DefView);
        winids.insert(winid_workerw);
    }

    // 隐藏窗口，不让mainwindow被挡住
    ShowWindow(winid_workerw, 0);

    winid_FloderView = FindWindowEx(winid_DefView, NULL, L"SysListView32", nullptr);
    qInfo() << "winid_FloderView:" << winid_FloderView << "winid_progman:" << winid_progman;

    winids.insert(winid_progman);
    winids.insert(winid_FloderView);
    return true;
}

void run()
{
    hookThread = std::thread([&] {
        initDesktopHwnd();
        startHook();
        thread_id = GetCurrentThreadId();
        MSG msg;
        while (GetMessage(&msg, NULL, 0, 0)) {
            if (msg.message == Exit_Hook_Thread) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        stopHook();
    });
    hookThread.detach();
}

void exit()
{
    qInfo() << "exit post msg";
    PostThreadMessage(thread_id, Exit_Hook_Thread, 0, 0);
}
} // namespace Hooker
