#include <windows.h>
#include <iostream>
#include <string>

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

int main() {
    //------------------------------------------------------
    // Load settings
    //------------------------------------------------------
    Settings settings;
    if (!settings.load(getConfigPath())) {
        std::wcout << L"Failed to load settings.json, using defaults.\n";
    }

    //------------------------------------------------------
    // Initialize controllers
    //------------------------------------------------------
    GammaControl gamma;           // gamma + contrast
    VibranceControl vib;          // digital vibrance

    bool vibReady = vib.init();   // gracefully handles failure

    int cur, mn, mx;

    if (vibReady && vib.getLevel(cur, mn, mx)) {
        std::cout << "DV CURRENT = " << cur << "\n";
        std::cout << "DV MIN     = " << mn << "\n";
        std::cout << "DV MAX     = " << mx << "\n";
    }

    bool active = false;          // toggle state

    //------------------------------------------------------
    // Define lambda helpers
    //------------------------------------------------------
    auto applyAll = [&]() {
        gamma.apply(settings.gamma_on, settings.contrast_on);
        if (vibReady) vib.setLevel(dvPercentToNvapi(settings.dv_on));
    };

    auto restoreAll = [&]() {
        gamma.apply(settings.gamma_off, settings.contrast_off);
        if (vibReady) vib.setLevel(dvPercentToNvapi(settings.dv_off));
    };


    //------------------------------------------------------
    // UI
    //------------------------------------------------------
    std::cout << "DarknessToggle (Gamma + Contrast + DV)\n";
    std::cout << "F3  = toggle ON/OFF\n";
    std::cout << "F5  = reload settings.json\n";
    std::cout << "F12 = exit (restore defaults)\n";

    //------------------------------------------------------
    // Main loop
    //------------------------------------------------------
    while (true) {

        // -------------------------
        // Toggle ON/OFF
        // -------------------------
        if (GetAsyncKeyState(VK_F3) & 1) {
            active = !active;
            if (active) applyAll();
            else        restoreAll();
        }


        // F5 = apply OFF state
        // -------------------------
        if (GetAsyncKeyState(VK_F5) & 1) {

            std::cout << "Reloading settings.json + applying OFF values\n";

            settings.load(getConfigPath());  // optional
            gamma.apply(settings.gamma_off, settings.contrast_off);

            if (vibReady)
                vib.setLevel(dvPercentToNvapi(settings.dv_off));

            active = false;
        }

        // -------------------------
        // Exit
        // -------------------------
        if (GetAsyncKeyState(VK_F12) & 1) {
            restoreAll();
            return 0;
        }

        Sleep(20);
    }
}
