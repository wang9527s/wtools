@echo off

rd /s /q build 
mkdir build
cd build

setlocal

set QT_DIR=C:\Qt\6.5.3\mingw_64
set CMAKE_DIR=C:\Qt\Tools\mingw1120_64\bin
set PATH=%QT_DIR%\bin;%CMAKE_DIR%;%PATH%

:: 使用 MinGW 生成器配置项目
cmake -G "MinGW Makefiles" ..

:: 使用 MinGW 构建项目 
mingw32-make -j20

endlocal

# windeployqt.exe 打包的库不全
# 将exe放到 C:\Qt\6.5.3\mingw_64\bin 中可以正常运行