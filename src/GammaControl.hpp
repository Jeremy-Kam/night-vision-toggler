#pragma once
#include <windows.h>
#include <array>

class GammaControl {
public:
    void save();
    void apply(double gamma, int contrast);
    void restore();
    void resaveOriginal();

private:
    bool saved = false;
    std::array<WORD, 3 * 256> original{};
};
