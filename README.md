# Night Vision Toggler

Night Vision Toggler is a lightweight Windows tool that lets you instantly toggle **gamma**, **contrast**, and **digital vibrance** for better visibility in dark game environments.  
The program runs fully in the background with no console window and responds instantly to hotkeys.

This project expands on the original GammaToggler by adding contrast and NVIDIA digital vibrance support. You will also notice some parts of the README are similar because they are similar programs.

---

## Features

- **Toggle ON/OFF** with a single key
- **Gamma adjustment** via GDI gamma ramp
- **Contrast adjustment** using a custom software curve
- **Digital Vibrance** (NVIDIA-only) via NVAPI dynamic loading
- **Configurable** using `settings.json`
- **Portable** — single executable, no install required
- Runs completely **in the background**

---

## Hotkeys

| Key   | Action |
|-------|--------|
| **F3**  | Toggle ON/OFF |
| **F5**  | Reload `settings.json` and apply **OFF** state |
| **F12** | Exit the program and restore OFF/default values |

---

## Configuration

Settings are stored in config/settings.json

## How to Build
Requires MinGW or another GCC-based compiler.

### Installing MinGW
If you do not have MinGW installed, you can follow [this YouTube video](https://www.youtube.com/watch?v=DamuE8TM3xo) (not my video, but I have followed his guides for a while).

The short of it is:
1. go to install [MingGW](https://sourceforge.net/projects/mingw/files/).
2. Use all defaults until you get to a page with checkboxes. Make sure mingw-developer-tools, mingw32-base, mingw32-gcc-g++, msys-base are checked (it will check other things as well). 
3. Go to Installation -> Apply Changes. Then click Apply.
4. Edit the environment variables (using Windows search).
5. Edit Path, add new entries: ```C:\MinGW\bin``` and ```C:\MinGW\MSYS\1.0\bin```
6. To test if it worked, go to command prompt and type in ```g++ --version``` and you should see an output stating the version you have. If not, something went wrong and you should probably watch the video.

## Compiling the Program
Run the ```build.bat``` file.

## Usage
Originally developed for using it with ARC Raiders or other competitive video games with dark environments, where having a higher gamma can give you an advantage. Morally, this is in a grey area, but it doesn't mess with the game itself. All it does is change the display, which anyone can do natively using NVIDIA Control Panel. You could also do something similar by making sure the room is as dark as possible and making the screen as bright as possible. I would recommend pinning the executable to start, so you can quickly start it up when starting up the game, and it will run in the background until you press F12.

## Credits
For the digital vibrance part of the code, I adapted the code from DVC2 from DavidLekei. They just set the vibrance to max for CS:GO, my code only bumps it up a little bit (50 -> 60), which is recommended for Tarkov.
