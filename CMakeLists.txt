cmake_minimum_required(VERSION 3.7)

set(TARGET wtools)
project(${TARGET})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(EXECUTABLE_OUTPUT_PATH ".")

set(SOURCES
    src/main/main.cpp
    src/main/widget.cpp
    src/tool/wtool.cpp
    src/tool/log.cpp
    src/icon/iconwidget.cpp
    src/timerTask/timertask.cpp
    src/timerTask/timertaskctrl.cpp
    src/timerTask/timertaskinfo.cpp
    src/x11opacity/opacityWidget.cpp
    src/rss/rssupdateopml.cpp
    src/rss/rsswidget.cpp
    src/tool/wconfig.cpp
    src/topTip/toptip.cpp
    src/watermark/watermark.cpp
    )
include_directories(.
    #/usr/include/x86_64-linux-gnu/wangdevtest/
    )
set(HEADERS
    src/main/widget.h
    src/tool/wconfig.h
    src/tool/wtool.h
    src/tool/customtabstyle.h
    src/tool/log.h
    src/tool/header.h
    src/icon/iconwidget.h
    src/timerTask/timertask.h
    src/timerTask/timertaskctrl.h
    src/timerTask/timertaskinfo.h
    src/x11opacity/opacityWidget.h
    src/x11opacity/windowsctrl.h
    src/rss/rssupdateopml.h
    src/rss/rsswidget.h
    src/topTip/toptip.h
    src/watermark/watermark.h
    )
set(FORMS
    src/main/widget.ui
    src/icon/iconwidget.ui
    src/timerTask/timertask.ui
    src/rss/rsswidget.ui
    )


set(QT Core Gui Widgets )

find_package(Qt5 REQUIRED ${QT})


# 生成的是可执行文件
add_executable(${TARGET} ${SOURCES} ${HEADERS} ${FORMS})

# 依赖库的位置
target_link_libraries(${TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        )

#deb
install(TARGETS ${TARGET} DESTINATION /opt/apps/wtools/)
install(FILES files/wtools.desktop DESTINATION share/applications/)
install(FILES files/wtools.desktop DESTINATION /etc/xdg/autostart)
install(FILES files/wtools.svg DESTINATION /opt/apps/wtools/)
