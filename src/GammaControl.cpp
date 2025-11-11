#include "GammaControl.hpp"
#include <cmath>

void GammaControl::save() {
    if (saved) return;
    HDC hdc = GetDC(nullptr);
    GetDeviceGammaRamp(hdc, original.data());
    ReleaseDC(nullptr, hdc);
    saved = true;
}

void GammaControl::apply(double gammaValue, int contrastValue) {
    save();

    std::array<WORD, 3 * 256> lut{};
    double invGamma = 1.0 / gammaValue;

    // ✅ Soft contrast mapping (50 = neutral)
    double contrastFactor = 1.0 + ((contrastValue - 50.0) / 200.0);

    for (int i = 0; i < 256; i++) {
        double x = i / 255.0;

        // 1. gamma correction
        double g = std::pow(x, invGamma);

        // 2. software contrast curve (centered at 0.5)
        double c = ((g - 0.5) * contrastFactor) + 0.5;

        // clamp
        if (c < 0.0) c = 0.0;
        if (c > 1.0) c = 1.0;

        WORD w = (WORD)(c * 65535.0);
        lut[i] = lut[256+i] = lut[512+i] = w;
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
