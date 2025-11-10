#include "VibranceControl.hpp"
#include <cstdio>
#include <cstring>

// Offsets (DVC2 constants)
static const unsigned int OFFS_NvAPI_Initialize             = 0x0150E828;
static const unsigned int OFFS_NvAPI_EnumNvidiaDisplayHandle= 0x9ABDD40D;
static const unsigned int OFFS_NvAPI_GetDVCInfo             = 0x4085DE45;
static const unsigned int OFFS_NvAPI_SetDVCLevel            = 0x172409B4;

// NV_DISPLAY_DVC_INFO (minimal) – matches DVC2 expectations
#pragma pack(push, 1)
struct NV_DISPLAY_DVC_INFO {
    unsigned int version;   // = sizeof(NV_DISPLAY_DVC_INFO) | 0x10000
    int currentDV;          // current level
    int minDV;              // min (often 0)
    int maxDV;              // max (often 63 or 100 depending on GPU/driver)
    // struct is larger in real NVAPI; we only need these fields here
};
#pragma pack(pop)

bool VibranceControl::loadDll() {
    // Prefer 64-bit DLL on 64-bit builds; fall back to generic
    hNvApi = LoadLibraryW(L"nvapi64.dll");
    if (!hNvApi) hNvApi = LoadLibraryW(L"nvapi.dll");
    return hNvApi != nullptr;
}

bool VibranceControl::resolveFunctions() {
    auto pQI = (NvAPI_QueryInterface_t)GetProcAddress(hNvApi, "nvapi_QueryInterface");
    if (!pQI) return false;
    NvAPI_QueryInterface = pQI;

    NvAPI_Initialize = (NvAPI_Initialize_t)NvAPI_QueryInterface(OFFS_NvAPI_Initialize);
    NvAPI_EnumNvidiaDisplayHandle = (NvAPI_EnumNvidiaDisplayHandle_t)NvAPI_QueryInterface(OFFS_NvAPI_EnumNvidiaDisplayHandle);
    NvAPI_GetDVCInfo = (NvAPI_GetDVCInfo_t)NvAPI_QueryInterface(OFFS_NvAPI_GetDVCInfo);
    NvAPI_SetDVCLevel = (NvAPI_SetDVCLevel_t)NvAPI_QueryInterface(OFFS_NvAPI_SetDVCLevel);

    return NvAPI_Initialize && NvAPI_EnumNvidiaDisplayHandle && NvAPI_GetDVCInfo && NvAPI_SetDVCLevel;
}

bool VibranceControl::enumDisplay0() {
    if (!NvAPI_EnumNvidiaDisplayHandle) return false;
    int status = NvAPI_EnumNvidiaDisplayHandle(0, &displayHandle);
    return status == 0;
}

bool VibranceControl::init() {
    if (!loadDll()) return false;
    if (!resolveFunctions()) return false;
    if (NvAPI_Initialize() != 0) return false;
    if (!enumDisplay0()) return false;

    // read original/current DV
    int cur, mn, mx;
    if (getLevel(cur, mn, mx)) {
        dv_current = cur; dv_min = mn; dv_max = mx;
        dv_original = cur; haveOriginal = true;
    }
    return true;
}

bool VibranceControl::getLevel(int& current, int& min, int& max) {
    if (!NvAPI_GetDVCInfo) return false;

    NV_DISPLAY_DVC_INFO info{};
    info.version = sizeof(NV_DISPLAY_DVC_INFO) | 0x10000;

    int status = NvAPI_GetDVCInfo(displayHandle, 0, &info);
    if (status != 0) return false;

    current = info.currentDV;
    min = info.minDV;
    max = info.maxDV;
    return true;
}

bool VibranceControl::setLevel(int level) {
    if (!NvAPI_SetDVCLevel) return false;

    // clamp against known range; if we haven't read, use sane defaults
    int mn = dv_min, mx = dv_max;
    if (mn >= mx || mx <= 0) { mn = 0; mx = 100; }
    if (level < mn) level = mn;
    if (level > mx) level = mx;

    int status = NvAPI_SetDVCLevel(displayHandle, 0, level);
    if (status == 0) {
        dv_current = level;
        return true;
    }
    return false;
}

void VibranceControl::restoreOriginalIfRequested(int dvOff) {
    if (dvOff >= 0) {
        setLevel(dvOff);
        return;
    }
    if (haveOriginal) setLevel(dv_original);
}
