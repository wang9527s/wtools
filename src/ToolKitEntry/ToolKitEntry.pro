QT       += core gui widgets

CONFIG += c++17

CONFIG      += console

TARGET = app_ToolKitEntry
RC_ICONS = ./img/app.ico
DESTDIR = $$PWD/../../bin

SOURCES += \
    setting/SettingWidget.cpp \
    main.cpp \
    systemtray.cpp

HEADERS += \
    CallExternal.h \
    setting/SettingConfig.h \
    setting/SettingWidget.h \
    systemtray.h

FORMS +=

RESOURCES += \
    qrc.qrc

include($$PWD/../utils/utils.pri)
include($$PWD/../third/QHotkey/QHotkey.pri)
