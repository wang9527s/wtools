#include "dump_helper.h"

#include <QDateTime>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

#include <DbgHelp.h>
#include <iostream>
#include <signal.h>
#include <new.h>

#pragma comment(lib, "dbghelp.lib")

namespace dump_handle
{

QString dump_filepath;
QString dump_basename;

LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
    QString str = QString("%1/%2-%3.dmp")
                      .arg(dump_filepath)
                      .arg(dump_basename)
                      .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh_mm_ss"));
    qInfo() << "slog start write to" << str;
    const QChar *path = str.unicode();
    HANDLE hFile = CreateFile((LPCWSTR)path, // L"mini.dmp",
                              GENERIC_READ | GENERIC_WRITE,
                              FILE_SHARE_WRITE,
                              NULL,
                              CREATE_ALWAYS,
                              FILE_ATTRIBUTE_NORMAL,
                              NULL);

    if (hFile == INVALID_HANDLE_VALUE) {
        qInfo() << "slog Create dump failed";
        return EXCEPTION_EXECUTE_HANDLER;
    }

    MINIDUMP_EXCEPTION_INFORMATION mdei;
    mdei.ThreadId = GetCurrentThreadId();
    mdei.ExceptionPointers = pException;
    mdei.ClientPointers = NULL;
    MINIDUMP_CALLBACK_INFORMATION mci;
    mci.CallbackRoutine = NULL;
    mci.CallbackParam = 0;

    // 数组越界的情况下 dump可能会生成失败
    MiniDumpWriteDump(
        GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, &mdei, NULL, &mci);

    CloseHandle(hFile);

    EXCEPTION_RECORD *record = pException->ExceptionRecord;
    qInfo() << "  errCode: " << QString::number(record->ExceptionCode, 16)
            << ", errAddr: " << QString::number((uint)record->ExceptionAddress, 16)
            << ", errFlag: " << QString::number(record->ExceptionFlags, 16)
            << ", errPara: " << QString::number(record->NumberParameters, 16);

    qInfo() << "slog creat dump " << (QFileInfo::exists(str) ? "success" : "failed");

    return EXCEPTION_EXECUTE_HANDLER;
}

void func_invalidParam_handler(const wchar_t *expression,
                               const wchar_t *function,
                               const wchar_t *file,
                               unsigned int line,
                               uintptr_t pReserved)
{
    qInfo() << "func_invalidParam_handler";
    qInfo() << "line:" << line;
    qInfo() << function;
    qInfo() << expression;
    throw std::invalid_argument("");
}

void func_sig_handler(int sig)
{
    qInfo() << "recv system signal: " << sig;
}

void start(QString filepath, QString basename)
{
    QDir dir;
    dir.mkdir(filepath);
    dump_filepath = filepath;
    dump_basename = basename;
    SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);

    // 捕获系统信号
    signal(SIGINT, func_sig_handler);
    signal(SIGILL, func_sig_handler);
    signal(SIGFPE, func_sig_handler);
    signal(SIGSEGV, func_sig_handler);
    signal(SIGTERM, func_sig_handler);
    signal(SIGBREAK, func_sig_handler);
    signal(SIGABRT, func_sig_handler);

    // C++ 运行时异常处理  （预计没用，先放着吧）
    /* 当throw的异常没有捕获的时候，会调用set_terminate设置的函数
     * SetUnhandledExceptionFilter设置后，程序throw (2)
     *    set_terminate 和 signal设置的函数，都无法捕获到异常。
     *    如果SetUnhandledExceptionFilter没设置，功能正常
     */
    // 在多线程环境中，每个线程的unexpected()和terminate()函数是单独维护的
    set_terminate([]() -> void { qInfo() << "slog recv terminal"; });
    // 没有被触发
    set_unexpected([]() -> void { qInfo() << "slog recv unexpected"; });

    // C 运行时库 (CRT) 异常处理
    _set_invalid_parameter_handler(func_invalidParam_handler);
    _set_purecall_handler([]() -> void { qInfo() << "slog recv purecall_handler"; });
    _set_new_handler([](size_t s) -> int {
        qInfo() << "slog recv _set_new_handler" << s;
        return s;
    });
    // 设置_set_abort_behavior后，发出abort后，程序立刻crashed
    _set_abort_behavior(0, _WRITE_ABORT_MSG);
}

} // namespace dump_handle
