@echo off
REM Build script for Visual Cryptography Project (Windows)
REM Reorganized folder structure

echo ===============================================
echo Visual Cryptography Project - Build Script
echo ===============================================
echo.

REM Check if g++ is available
where g++ >nul 2>nul
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: g++ compiler not found!
    echo Please install MinGW or add it to your PATH.
    pause
    exit /b 1
)

echo Compiler found: 
g++ --version | findstr "g++"
echo.

REM Create bin directory if it doesn't exist
if not exist "bin" mkdir bin

echo Building main program...
g++ -std=c++11 -Wall -O2 -o bin\vc_program.exe ^
    src\main.cpp src\image_utils.cpp src\vcs.cpp src\rg.cpp src\dhcod.cpp
if %ERRORLEVEL% NEQ 0 goto :error

echo Building analysis tool...
g++ -std=c++11 -Wall -O2 -o bin\analyze.exe ^
    src\analyze.cpp src\image_utils.cpp src\vcs.cpp src\rg.cpp src\dhcod.cpp
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo ===============================================
echo BUILD SUCCESSFUL!
echo ===============================================
echo.
echo Executables created in bin\ directory:
echo   - vc_program.exe  (Main program)
echo   - analyze.exe     (Analysis tool)
echo.
echo To run:
echo   bin\vc_program.exe
echo   bin\analyze.exe
echo.
pause
exit /b 0

:error
echo.
echo ===============================================
echo BUILD FAILED!
echo ===============================================
pause
exit /b 1
