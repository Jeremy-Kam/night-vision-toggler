@echo off
mkdir build >nul 2>&1
echo Building DarknessToggle...

rem No static NVAPI library needed; we load nvapi64.dll at runtime.
g++ src/*.cpp -o build/DarknessToggle.exe -lgdi32 -luser32

echo Done. The EXE is in build\DarknessToggle.exe
pause
