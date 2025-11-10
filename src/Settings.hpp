#pragma once
#include <string>

struct Settings {
    double gamma_on = 2.19;
    double gamma_off = 1.0;

    int contrast_on = 55;
    int contrast_off = 50;

    int dv_on = 60;
    int dv_off = 50;

    bool load(const std::wstring& path);
};
