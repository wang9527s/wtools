@echo off

set CLANG_FORMAT="C:\Program Files\LLVM\bin\clang-format.exe"
set SRC_FOLDER=%~dp0
set CONFIG_FILE=%~dp0\.clang-format

for /R "%SRC_FOLDER%" %%i in (*.cpp *.h) do (
	echo %%i | findstr /R /C:"\\build-" > nul
	if not errorlevel 1 (
		rem 忽略路径中包含  \build-   的
	) else (
		%CLANG_FORMAT% --style=file:"%CONFIG_FILE%" -i --verbose "%%i"
	)
	echo "" > nul
)