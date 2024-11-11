#include "StdInc.h"

#include "CustomCarEnvMapPipeline.h"

// 0x5D8D30
void* CCustomCarEnvMapPipeline::pluginEnvAtmConstructorCB(void* object, int32 offsetInObject, int32 sizeInObject) {
    EnvMapAtmPlGetData(object) = nullptr;
    return object;
}

// 0x5D9730
void* CCustomCarEnvMapPipeline::pluginEnvAtmDestructorCB(void* object, int32 offsetInObject, int32 sizeInObject) {
    auto*& data = EnvMapAtmPlGetData(object);
    if (data) {
        m_gEnvMapPipeAtmDataPool->Delete(std::exchange(data, nullptr));
    }
    return object;
}

// 0x5D9780
void* CCustomCarEnvMapPipeline::pluginEnvAtmCopyConstructorCB(void* dstObject, const void* srcObject, int32 offsetInObject, int32 sizeInObject) {
    return plugin::CallAndReturn<void*, 0x5D9780, void*, const void*, int32, int32>(dstObject, srcObject, offsetInObject, sizeInObject);
}

// 0x5D96F0
CustomEnvMapPipeAtomicData* CCustomCarEnvMapPipeline::AllocEnvMapPipeAtomicData(RpAtomic* atomic) {
    auto*& data = EnvMapAtmPlGetData(atomic);
    if (!data) {
        if (data = m_gEnvMapPipeAtmDataPool->New()) {
            SetCustomEnvMapPipeAtomicDataDefaults(data);
        }
    }
    return data;
}

void CCustomCarEnvMapPipeline::SetCustomEnvMapPipeAtomicDataDefaults(CustomEnvMapPipeAtomicData* data) {
    data->offsetU = 0.0f;
    data->prevPosY = 0.0f;
    data->prevPosX = 0.0f;
}
