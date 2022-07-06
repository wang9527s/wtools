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
    )
include_directories(
	.
    )


set(QT Core Gui Widgets )

find_package(Qt5 REQUIRED ${QT})



# 生成的是可执行文件
add_executable(${TARGET} ${SOURCES})

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
