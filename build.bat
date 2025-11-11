@echo off
mkdir build >nul 2>&1
echo Building DarknessToggle...

g++ src/*.cpp -o build/DarknessToggle.exe -lgdi32 -luser32 -mwindows

echo.
echo Done. The EXE is in build\DarknessToggle.exe
pause
