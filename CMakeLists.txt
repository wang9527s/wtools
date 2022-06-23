cmake_minimum_required(VERSION 3.7)

set(TARGET wtools)
project(${TARGET})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(EXECUTABLE_OUTPUT_PATH ".")

set(QT Core Gui Widgets )
find_package(Qt5 REQUIRED ${QT})

# x11
find_package(PkgConfig REQUIRED)
find_package(Qt5X11Extras REQUIRED)
pkg_check_modules(XCB_LIBS REQUIRED xcb-ewmh xcb xcb-image xcb-composite xtst x11 xext xcb-icccm dbusmenu-qt5)

include_directories(
    .
    )

# 添加源码
aux_source_directory(${PROJECT_SOURCE_DIR}/src/main SRCMAIN)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/tool SRCTOOL)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/icon SRCICON)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/timerTask SRCTIMERTASK)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/x11opacity SRCXOPACITY)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/rss SRCRSS)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/topTip SRCTOOLTIP)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/watermark SRCWATERMASK)

# 生成的是可执行文件
add_executable(${TARGET}
    ${SRCMAIN}
    ${SRCTOOL}
    ${SRCICON}
    ${SRCTIMERTASK}
    ${SRCXOPACITY}
    ${SRCRSS}
    ${SRCTOOLTIP}
    ${SRCWATERMASK}
    )

# 依赖库的位置
target_link_libraries(${TARGET}
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        ${Qt5X11Extras_LIBRARIES}
        ${XCB_LIBS_LIBRARIES}
        )

#deb
install(TARGETS ${TARGET} DESTINATION /opt/apps/wtools/)
install(FILES files/wtools.desktop DESTINATION share/applications/)
install(FILES files/wtools.desktop DESTINATION /etc/xdg/autostart)
install(FILES files/wtools.svg DESTINATION /opt/apps/wtools/)
