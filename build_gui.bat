@echo off
rem Build script for srms_gui.exe
echo Attempting to build with g++ (MinGW)...
g++ srms_gui.cpp -o srms_gui.exe -static -lgdi32 -lmsimg32
if %ERRORLEVEL% EQU 0 (
    echo Built srms_gui.exe with g++.
    goto :end
)

echo g++ build failed or not found. Attempting Visual Studio cl.exe...
cl /EHsc srms_gui.cpp user32.lib gdi32.lib
if %ERRORLEVEL% EQU 0 (
    echo Built srms_gui.exe with cl.exe.
    goto :end
)

echo Build failed. Please ensure you have either MinGW (g++) or Visual Studio (cl) installed and in PATH.
:end
echo Done.
pause
