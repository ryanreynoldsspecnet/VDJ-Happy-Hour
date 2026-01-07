#include "pch.h"

#include "HappyHourPlugin.h"
#include "resource.h"

#include <cstring>
#include <cstdio>

static const GUID CLSID_HappyHourDsp =
{ 0xa0d3c11f, 0x4ef9, 0x4fd1, { 0x90, 0x64, 0x8b, 0x53, 0x45, 0x0a, 0x58, 0xb2 } };

// Minimal COM-style factory used by VirtualDJ
VDJ_EXPORT HRESULT VDJ_API DllGetClassObject(const GUID& rclsid, const GUID& riid, void** ppObject)
{
    if (!ppObject) return E_FAIL;
    *ppObject = nullptr;

    if (rclsid.Data1 != CLSID_HappyHourDsp.Data1 ||
        rclsid.Data2 != CLSID_HappyHourDsp.Data2 ||
        rclsid.Data3 != CLSID_HappyHourDsp.Data3 ||
        std::memcmp(rclsid.Data4, CLSID_HappyHourDsp.Data4, sizeof(CLSID_HappyHourDsp.Data4)) != 0)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    if (riid.Data1 != IID_IVdjPluginDsp8.Data1 ||
        riid.Data2 != IID_IVdjPluginDsp8.Data2 ||
        riid.Data3 != IID_IVdjPluginDsp8.Data3 ||
        std::memcmp(riid.Data4, IID_IVdjPluginDsp8.Data4, sizeof(IID_IVdjPluginDsp8.Data4)) != 0)
    {
        return CLASS_E_CLASSNOTAVAILABLE;
    }

    *ppObject = static_cast<IVdjPluginDsp8*>(new HappyHourDsp());
    return S_OK;
}

HRESULT VDJ_API HappyHourDsp::OnLoad()
{
    // Parameters (Phase 1): declare only; do not apply any DSP behavior yet.
    enabled_ = 0;
    duck_level_ = 0.30f;
    fade_time_ = 1.0f;

    DeclareParameterSwitch(&enabled_, PARAM_ENABLED, "Enabled", "Enabled", false);
    DeclareParameterSlider(&duck_level_, PARAM_DUCK_LEVEL, "Duck level", "Duck", 0.30f);
    DeclareParameterSlider(&fade_time_, PARAM_FADE_TIME, "Fade time", "Fade", 1.0f);

    return S_OK;
}

HRESULT VDJ_API HappyHourDsp::OnGetPluginInfo(TVdjPluginInfo8* info)
{
    if (!info) return E_FAIL;

    info->PluginName = "Happy Hour";
    info->Author = "SpecNet";
    info->Description = "DSP Sound Effect (Phase 0)";
    info->Version = "0.0";
    info->Bitmap = nullptr;
    info->Flags = 0;

    return S_OK;
}

bool HappyHourDsp::LoadFileFromResource(int resourceId, const char* resourceType, const void** data, int* size)
{
#ifdef VDJ_WIN
    if (!data || !size) return false;
    *data = nullptr;
    *size = 0;

    HINSTANCE inst = GetModuleHandleW(L"VDJ Happy Hour.dll");
    if (!inst) inst = (HINSTANCE)GetModuleHandleW(nullptr);

    HRSRC hRes = FindResourceA(inst, MAKEINTRESOURCEA(resourceId), resourceType);
    if (!hRes) return false;

    HGLOBAL hResData = LoadResource(inst, hRes);
    if (!hResData) return false;

    DWORD resSize = SizeofResource(inst, hRes);
    if (!resSize) return false;

    void* resPtr = LockResource(hResData);
    if (!resPtr) return false;

    *data = resPtr;
    *size = (int)resSize;
    return true;
#else
    (void)resourceId;
    (void)resourceType;
    (void)data;
    (void)size;
    return false;
#endif
}

HRESULT VDJ_API HappyHourDsp::OnGetUserInterface(TVdjPluginInterface8* pluginInterface)
{
    if (!pluginInterface) return E_FAIL;

    pluginInterface->Type = VDJINTERFACE_SKIN;

    const void* xml = nullptr;
    int xmlSize = 0;
    if (!LoadFileFromResource(IDR_SKIN_XML, "XML", &xml, &xmlSize) || !xml || xmlSize <= 0)
    {
        pluginInterface->Xml = nullptr;
        pluginInterface->ImageBuffer = nullptr;
        pluginInterface->ImageSize = 0;
        return E_FAIL;
    }

    pluginInterface->Xml = (const char*)xml;
    pluginInterface->ImageBuffer = nullptr;
    pluginInterface->ImageSize = 0;

    return S_OK;
}

HRESULT VDJ_API HappyHourDsp::OnParameter(int id)
{
    // Parameters are stored via DeclareParameter* bindings.
    // No DSP behavior yet.
    (void)id;
    return S_OK;
}

HRESULT VDJ_API HappyHourDsp::OnGetParameterString(int id, char* outParam, int outParamSize)
{
    if (!outParam || outParamSize <= 0) return E_FAIL;

    switch (id)
    {
    case PARAM_ENABLED:
        std::snprintf(outParam, (size_t)outParamSize, "%s", enabled_ ? "On" : "Off");
        return S_OK;
    case PARAM_DUCK_LEVEL:
        std::snprintf(outParam, (size_t)outParamSize, "%.2f", duck_level_);
        return S_OK;
    case PARAM_FADE_TIME:
        std::snprintf(outParam, (size_t)outParamSize, "%.2fs", fade_time_);
        return S_OK;
    default:
        break;
    }

    return E_NOTIMPL;
}

HRESULT VDJ_API HappyHourDsp::OnProcessSamples(float* buffer, int nb)
{
    (void)buffer;
    (void)nb;
    return S_OK;
}
