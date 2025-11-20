# Night Vision Toggler

Night Vision Toggler is a lightweight Windows tool that lets you instantly toggle **gamma**, **contrast**, and **digital vibrance** for better visibility in dark game environments.  
The program runs fully in the background with no console window and responds instantly to hotkeys.

This project expands on the original GammaToggler by adding contrast and NVIDIA digital vibrance support.

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

