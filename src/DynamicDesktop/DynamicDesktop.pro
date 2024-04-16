QT       += core gui widgets concurrent

CONFIG += c++17

TARGET = DynamicDesktop
DESTDIR = $$PWD/../../bin

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
    windows_platform.h

RESOURCES += \
    resource/img/img.qrc

include($$PWD/../utils/utils.pri)

