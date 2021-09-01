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

// TODO: Find out what CRANE_MAGNET1/2/3/4 is exactly..
enum class eRopeType {
    NONE,
    CRANE_MAGNET1,
    CRANE_HARNESS,
    MAGNET,
    CRANE_MAGNET2,
    WRECKING_BALL,
    CRANE_MAGNET3,
    CRANE_MAGNET4
};

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
    CPhysical* m_pAttachedEntity;
    CPhysical* m_pRopeAttachObject;
    float    m_segmentLength;
    uint32   m_nTime;
    uint8    m_nSegments;
    eRopeType    m_type;
    uint8    m_nFlags1;
    uint8    m_nFlags2;

public:
    static void InjectHooks();

    void ReleasePickedUpObject();
    ModelIndex GetModelForType() const;
    void CreateHookObjectForRope();
    int8 UpdateWeightInRope(float a2, float a3, float a4, int32 a5, float* a6);
    void Remove();
    void Render();
    void PickUpObject(CPhysical* obj);
    void Update();
};
VALIDATE_SIZE(CRope, 0x328);
