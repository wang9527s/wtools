QT += core

# 遇到一个问题，ide中有输出，如果在终端中命令行启动，则没有输出
#   使用环境变量、std::cout、C语言库函数，都没有作用，终端中没有输出
#   最后 CONFIG += cmdline 解决问题了
CONFIG += c++17 cmdline
DESTDIR = $$PWD/../../../bin

SOURCES += \
    utf8_2_utf8bom.cpp

include($$PWD/encoding.pri)
