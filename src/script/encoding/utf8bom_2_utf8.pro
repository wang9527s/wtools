QT += core

CONFIG +=  c++17 cmdline
DESTDIR = $$PWD/../../../bin

SOURCES += \
    utf8bom_2_utf8.cpp

include($$PWD/encoding.pri)
