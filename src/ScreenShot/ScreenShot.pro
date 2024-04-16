QT       += core gui widgets

TARGET = ScreenShot8
DESTDIR = $$PWD/../../bin

CONFIG += c++11

SOURCES += \
    drawcontent.cpp \
    main.cpp \
    screenshots.cpp \
    selectshotarea.cpp

HEADERS += \
    Tool.h \
    drawcontent.h \
    screenshots.h \
    selectshotarea.h

RESOURCES += \
    img/icon.qrc

include($$PWD/../utils/utils.pri)
