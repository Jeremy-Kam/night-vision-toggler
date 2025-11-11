#include <windows.h>
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

#include "GammaControl.hpp"
#include "Settings.hpp"
#include "VibranceControl.hpp"

// --------------------------------------------------------
// Resolve "..\\config\\settings.json" relative to EXE path
// --------------------------------------------------------
static std::wstring getExeDir() {
    wchar_t buf[MAX_PATH];
    GetModuleFileNameW(nullptr, buf, MAX_PATH);
    std::wstring p(buf);
    return p.substr(0, p.find_last_of(L"\\/"));
}

static std::wstring getConfigPath() {
    return getExeDir() + L"\\..\\config\\settings.json";
}

// --------------------------------------------------------
// TRUE background mode using WinMain
// --------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // (No console exists, so prints do nothing but are harmless)

    Settings settings;
    settings.load(getConfigPath());

    GammaControl gamma;
    VibranceControl vib;

    bool vibReady = vib.init();
    bool active = false;

    while (true) {

        // F3 = toggle ON/OFF
        if (GetAsyncKeyState(VK_F3) & 1) {
            active = !active;

            if (active) {
                gamma.apply(settings.gamma_on, settings.contrast_on);
                if (vibReady) vib.setLevel(dvPercentToNvapi(settings.dv_on));
            } else {
                gamma.apply(settings.gamma_off, settings.contrast_off);
                if (vibReady) vib.setLevel(dvPercentToNvapi(settings.dv_off));
            }
        }

        // F5 = reload + force OFF state
        if (GetAsyncKeyState(VK_F5) & 1) {
            settings.load(getConfigPath());
            gamma.apply(settings.gamma_off, settings.contrast_off);
            if (vibReady) vib.setLevel(dvPercentToNvapi(settings.dv_off));
            active = false;
        }

        // F12 = exit and restore defaults
        if (GetAsyncKeyState(VK_F12) & 1) {
            gamma.apply(settings.gamma_off, settings.contrast_off);
            if (vibReady) vib.setLevel(dvPercentToNvapi(settings.dv_off));
            return 0;
        }

        Sleep(30);
    }
}
