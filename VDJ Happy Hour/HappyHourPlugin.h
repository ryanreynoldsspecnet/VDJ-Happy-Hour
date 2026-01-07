#pragma once

#include "vdjDsp8.h"

class HappyHourDsp : public IVdjPluginDsp8
{
public:
    HRESULT VDJ_API OnLoad() override;
    HRESULT VDJ_API OnGetPluginInfo(TVdjPluginInfo8* info) override;
    HRESULT VDJ_API OnGetUserInterface(TVdjPluginInterface8* pluginInterface) override;

    HRESULT VDJ_API OnStart() override { return S_OK; }
    HRESULT VDJ_API OnStop() override { return S_OK; }

    HRESULT VDJ_API OnProcessSamples(float* buffer, int nb) override;

    HRESULT VDJ_API OnParameter(int id) override;
    HRESULT VDJ_API OnGetParameterString(int id, char* outParam, int outParamSize) override;

private:
    static bool LoadFileFromResource(int resourceId, const char* resourceType, const void** data, int* size);

    enum ParamId
    {
        PARAM_ENABLED = 1,
        PARAM_DUCK_LEVEL = 2,
        PARAM_FADE_TIME = 3,
    };

    int enabled_ = 0;
    float duck_level_ = 0.30f;
    float fade_time_ = 1.0f;
};
