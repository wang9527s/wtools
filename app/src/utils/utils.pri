
HEADERS += \
    $$PWD/logger.hpp \
    $$PWD/instance_header.h \
    $$PWD/win/dump_helper.h \
    $$PWD/wtool.h

SOURCES += \
    $$PWD/win/dump_helper.cpp \
    $$PWD/wtool.cpp


unix {
    DEFINES += Plat_Linux
    message("linux")
}
win32 {
    DEFINES += Plat_Windows
    message("windows")_
}
