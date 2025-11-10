#include "Settings.hpp"
#include <fstream>
#include <sstream>
#include <windows.h>
#include <cctype>

// Convert std::wstring → UTF-8 std::string (MinGW-safe)
static std::string wide_to_utf8(const std::wstring& w) {
    if (w.empty()) return "";
    int size = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (size <= 0) return "";
    std::string out(size - 1, '\0'); // exclude null
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), -1, &out[0], size, nullptr, nullptr);
    return out;
}

static bool extractNumber(const std::string& json, const std::string& key, double& out) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return false;
    pos = json.find(":", pos);
    if (pos == std::string::npos) return false;
    pos++;
    while (pos < json.size() && std::isspace((unsigned char)json[pos])) pos++;
    std::stringstream ss;
    while (pos < json.size() && (std::isdigit((unsigned char)json[pos]) || json[pos]=='.' || json[pos]=='-' )) {
        ss << json[pos++];
    }
    ss >> out;
    return !ss.fail();
}

bool Settings::load(const std::wstring& path) {
    std::ifstream f(wide_to_utf8(path));
    if (!f.is_open()) return false;

    std::stringstream buf; buf << f.rdbuf();
    std::string json = buf.str();

    double v = 0;
    if (extractNumber(json, "gamma_on", v)) gamma_on = v;
    if (extractNumber(json, "gamma_off", v)) gamma_off = v;

    if (extractNumber(json, "contrast_on", v)) contrast_on = (int)v;
    if (extractNumber(json, "contrast_off", v)) contrast_off = (int)v;

    if (extractNumber(json, "digital_vibrance_on", v)) dv_on = (int)v;
    if (extractNumber(json, "digital_vibrance_off", v)) dv_off = (int)v;


    return true;
}
