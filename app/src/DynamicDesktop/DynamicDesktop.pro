QT       += core gui widgets concurrent openglwidgets

CONFIG += c++17

TARGET = app_DynamicDesktop
DESTDIR = $$PWD/../../bin

LIBS += -lUser32

SOURCES += \
    RotatingAlbums/graphicspixmap.cpp \
    RotatingAlbums/graphicsview.cpp \
    RotatingAlbums/rotatingalbums.cpp \
    desktop.cpp \
    func_header.cpp \
    glwidget.cpp \
    main.cpp \
    windows_platform.cpp

HEADERS += \
    AppMsg.h \
    ImagePathnameManager.h \
    RotatingAlbums/graphicspixmap.h \
    RotatingAlbums/graphicsview.h \
    RotatingAlbums/rotatingalbums.h \
    desktop.h \
    func_header.h \
    glwidget.h \
    watermarks.h \
    windows_platform.h

RESOURCES += \
    resource/img/img.qrc

include($$PWD/../utils/utils.pri)

