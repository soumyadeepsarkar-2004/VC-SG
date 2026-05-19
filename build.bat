@echo off
REM Build script for Visual Cryptography Project (Windows)
REM Builds: main program, analysis tool, test suite

echo ===============================================
echo Visual Cryptography Project (VC-SG) - Build
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

echo [1/3] Building main program...
g++ -std=c++11 -Wall -Wextra -O2 -o bin\vc_program.exe ^
    src\main.cpp src\image_utils.cpp src\vcs.cpp src\rg.cpp src\dhcod.cpp
if %ERRORLEVEL% NEQ 0 goto :error

echo [2/3] Building analysis tool...
g++ -std=c++11 -Wall -Wextra -O2 -o bin\analyze.exe ^
    src\analyze.cpp src\image_utils.cpp src\vcs.cpp src\rg.cpp src\dhcod.cpp
if %ERRORLEVEL% NEQ 0 goto :error

echo [3/3] Building test suite...
g++ -std=c++11 -Wall -Wextra -O2 -o bin\vc_test.exe ^
    src\test.cpp src\image_utils.cpp src\vcs.cpp src\rg.cpp src\dhcod.cpp
if %ERRORLEVEL% NEQ 0 goto :error

echo.
echo ===============================================
echo BUILD SUCCESSFUL!
echo ===============================================
echo.
echo Executables created in bin\ directory:
echo   - vc_program.exe  (Main program)
echo   - analyze.exe     (Analysis tool)
echo   - vc_test.exe     (Test suite)
echo.
echo To run:
echo   bin\vc_program.exe           Run all schemes
echo   bin\vc_program.exe -h        Show help
echo   bin\analyze.exe              Run analysis
echo   bin\analyze.exe --csv r.csv  Export results
echo   bin\vc_test.exe              Run tests
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
