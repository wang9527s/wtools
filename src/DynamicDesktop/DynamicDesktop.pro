QT       += core gui widgets concurrent

CONFIG += c++17

TARGET = DynamicDesktop
DESTDIR = $$PWD/../../bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
    RotatingAlbums/graphicspixmap.h \
    RotatingAlbums/graphicsview.h \
    RotatingAlbums/rotatingalbums.h \
    desktop.h \
    func_header.h \
    glwidget.h \
    windows_platform.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    img.qrc
