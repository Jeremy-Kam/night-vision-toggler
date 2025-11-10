#include "GammaControl.hpp"
#include <cmath>

void GammaControl::save() {
    if (saved) return;
    HDC hdc = GetDC(nullptr);
    GetDeviceGammaRamp(hdc, original.data());
    ReleaseDC(nullptr, hdc);
    saved = true;
}

void GammaControl::apply(double gamma, int contrast) {
    save();

    std::array<WORD, 3 * 256> lut{};
    double invGamma = 1.0 / gamma;
    double contrastScale = contrast / 50.0; // 50 = neutral

    for (int i = 0; i < 256; i++) {
        double v = std::pow(i / 255.0, invGamma) * contrastScale;
        if (v > 1.0) v = 1.0;
        WORD w = static_cast<WORD>(v * 65535.0);
        lut[i] = lut[256 + i] = lut[512 + i] = w;
    }

    HDC hdc = GetDC(nullptr);
    SetDeviceGammaRamp(hdc, lut.data());
    ReleaseDC(nullptr, hdc);
}

void GammaControl::restore() {
    if (!saved) return;
    HDC hdc = GetDC(nullptr);
    SetDeviceGammaRamp(hdc, original.data());
    ReleaseDC(nullptr, hdc);
}

void GammaControl::resaveOriginal() {
    saved = false;
    save();
}
