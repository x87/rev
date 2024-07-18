#pragma once

#include "Vector.h"

class CPlaceable;

struct sReverbEnvironment {
    uint32 m_nEnvironment;
    int32  m_nDepth;
};

class CAEAudioEnvironment {
public:
    static void InjectHooks();

    static float GetDopplerRelativeFrequency(float prevDist, float curDist, uint32 prevTime, uint32 curTime, float timeScale);
    static float GetDistanceAttenuation(float dist);
    static float GetDirectionalMikeAttenuation(const CVector& soundDir);
    static void  GetReverbEnvironmentAndDepth(int8* reverbEnv, int32* depth);

    static void GetPositionRelativeToCamera(CVector& out, const CVector* vecPos);
    static CVector GetPositionRelativeToCamera(const CVector& pos) { CVector o; GetPositionRelativeToCamera(o, &pos); return o; }

    static void GetPositionRelativeToCamera(CVector& out, CPlaceable* placeable);
    static CVector GetPositionRelativeToCamera(CPlaceable* placeable) { CVector o; GetPositionRelativeToCamera(o, placeable); return o; }
};

static constexpr int32 NUM_AUDIO_ENVIRONMENTS = 68;
extern sReverbEnvironment (&gAudioZoneToReverbEnvironmentMap)[NUM_AUDIO_ENVIRONMENTS];

static constexpr int32 NUM_SOUND_DIST_ATTENUATION_ENTRIES = 1280;
extern float (&gSoundDistAttenuationTable)[NUM_SOUND_DIST_ATTENUATION_ENTRIES];
