#pragma once
#include <windows.h>

// Minimal NVAPI DVC wrapper (DVC2-style)
class VibranceControl {
public:
    bool init();                              // load nvapi, init, grab default display handle, read original DV
    bool getLevel(int& current, int& min, int& max);
    bool setLevel(int level);                 // clamps to [min,max]
    void restoreOriginalIfRequested(int dvOff); // if dvOff >= 0 -> set dvOff; if dvOff < 0 -> restore original
private:
    // nvapi pointers
    using NvAPI_QueryInterface_t = void* (__cdecl*)(unsigned int);
    using NvAPI_Initialize_t = int (__cdecl*)();
    using NvAPI_EnumNvidiaDisplayHandle_t = int (__cdecl*)(int, int*);
    using NvAPI_GetDVCInfo_t = int (__cdecl*)(int /*hDisplay*/, int /*outputId*/, void* /*NV_DISPLAY_DVC_INFO* */);
    using NvAPI_SetDVCLevel_t = int (__cdecl*)(int /*hDisplay*/, int /*outputId*/, int /*level*/);

    // function pointers
    NvAPI_QueryInterface_t NvAPI_QueryInterface = nullptr;
    NvAPI_Initialize_t NvAPI_Initialize = nullptr;
    NvAPI_EnumNvidiaDisplayHandle_t NvAPI_EnumNvidiaDisplayHandle = nullptr;
    NvAPI_GetDVCInfo_t NvAPI_GetDVCInfo = nullptr;
    NvAPI_SetDVCLevel_t NvAPI_SetDVCLevel = nullptr;

    HMODULE hNvApi = nullptr;
    int displayHandle = 0;
    int dv_min = 0, dv_max = 100, dv_current = 50, dv_original = 50;
    bool haveOriginal = false;

    bool loadDll();
    bool resolveFunctions();
    bool enumDisplay0();
};
