#ifndef WINDOWS_PLATFORM_H
#define WINDOWS_PLATFORM_H

#include <Windows.h>

/*
 * HWND FindWindowEx(
        HWND    hwndParent,         指定父窗口的句柄。如果为
 nullptr，则函数从桌面窗口开始搜索顶级窗口。 HWND    hwndChildAfter,     指定在查找时在 Z
 顺序中的相对于哪个子窗口之后开始。可以为 nullptr，表示从头开始搜索。 LPCWSTR lpszClass,
 指定要查找的窗口类名（或类名的部分）。如果为
 nullptr，则函数将返回具有指定窗口名的第一个窗口，窗口名不区分大小写。 LPCWSTR lpszWindow
        );
 */

class Desktop_Wnds
{
public:
    HWND progman = NULL;
    HWND hFloderView = NULL;
    HWND hWorkerW = NULL; // WorkerW
    HWND hDefView = NULL; // SHELLDLL_DefView
    bool init();

public:
    static Desktop_Wnds *instance()
    {
        static Desktop_Wnds obj;
        return &obj;
    }

private:
    Desktop_Wnds &operator=(const Desktop_Wnds &) = delete;
};

#endif // WINDOWS_PLATFORM_H
