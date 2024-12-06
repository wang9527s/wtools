cmake_minimum_required(VERSION 3.7)

set(TARGET wtools)
project(${TARGET})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(EXECUTABLE_OUTPUT_PATH ".")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/files)    # 可执行程序目录

include_directories(
	.
    )

set(qrc_FILES ./src/pixmap/img.qrc)		# 添加资源文件

set(QT Core Gui Widgets Svg)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(CMAKE_CXX_FLAGS "-mwindows")
endif()

find_package(Qt5 REQUIRED ${QT})

set(WTool_SRC
    ./src/drawer/drawerwidget.cpp
    ./src/drawer/drawerwidget.h
    ./src/main/main.cpp
    ./src/main/widget.cpp
    ./src/main/widget.h
    ./src/rss/rssupdateopml.cpp
    ./src/rss/rssupdateopml.h
    ./src/rss/rsswidget.cpp
    ./src/rss/rsswidget.h
    ./src/rss/rsswidget.ui
    ./src/timerTask/timertask.cpp
    ./src/timerTask/timertask.h
    ./src/timerTask/timertask.ui
    ./src/timerTask/timertaskctrl.cpp
    ./src/timerTask/timertaskctrl.h
    ./src/timerTask/timertaskinfo.cpp
    ./src/timerTask/timertaskinfo.h
    ./src/tool/customtabstyle.h
    ./src/tool/header.h
    ./src/tool/log.cpp
    ./src/tool/log.h
    ./src/tool/wconfig.cpp
    ./src/tool/wconfig.h
    ./src/tool/wtool.cpp
    ./src/tool/wtool.h
    ./src/topTip/toptip.cpp
    ./src/topTip/toptip.h
    ./src/x11opacity/opacityWidget.cpp
    ./src/x11opacity/opacityWidget.h
    ./src/x11opacity/windowsctrl.cpp
    ./src/x11opacity/windowsctrl.h
)

# 生成的是可执行文件
add_executable(${TARGET}
    ${WTool_SRC}
#    ${qrc_FILES}				# 添加资源文件
	)

# 依赖库的位置
target_link_libraries(${TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        Qt5::Svg
        )

#deb
install(TARGETS ${TARGET} DESTINATION /opt/apps/wtools/)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    add_definitions(-DPlat_Linux="")                                 # 添加宏定义
    install(FILES files/wtools.desktop DESTINATION share/applications/)
    install(FILES files/wtools.desktop DESTINATION /etc/xdg/autostart)
    install(FILES files/wtools.svg DESTINATION /opt/apps/wtools/)
    install(PROGRAMS files/rss-bilibili DESTINATION /opt/apps/wtools)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    add_definitions(-DPlat_Windows="")
    set(CURRENT_SYSTEM "Windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
    set(CURRENT_SYSTEM "FreeBSD")
endif()



message(STATUS "Current System is " ${CURRENT_SYSTEM})

