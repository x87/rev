/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "PluginBase.h"
#include "CVector.h"

class CEntity;
class CObject;

constexpr auto NUM_ROPE_SEGMENTS{ 32u };
class  CRope {
public:
    CVector  m_segments[NUM_ROPE_SEGMENTS];
    CVector  m_segmentsReleased[NUM_ROPE_SEGMENTS];
    int      m_nId;
    float    field_304;
    float    m_mass;
    float    m_totalLength;
    CEntity* m_pRopeHolder;
    CEntity* m_pAttachedEntity;
    CObject* m_pRopeAttachObject;
    float    m_segmentLength;
    uint32   m_nTime;
    uint8    m_nSegments;
    uint8    m_type;
    uint8    m_nFlags1;
    uint8    m_nFlags2;

public:
    static void InjectHooks();

    void ReleasePickedUpObject();
    void CreateHookObjectForRope();
    int8 UpdateWeightInRope(float a2, float a3, float a4, int32 a5, float* a6);
    void Remove();
    void Render();
    void PickUpObject(CEntity* a2);
    void Update();
};
VALIDATE_SIZE(CRope, 0x328);
