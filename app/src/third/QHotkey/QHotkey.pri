HEADERS += $$PWD/qhotkey.h \
        $$PWD/qhotkey_p.h

SOURCES += $$PWD/qhotkey.cpp
LIBS += -lUser32

mac: SOURCES += $$PWD/qhotkey_mac.cpp
else:win32: SOURCES += $$PWD/qhotkey_win.cpp
else:unix: SOURCES += $$PWD/qhotkey_x11.cpp

