#include "StdInc.h"
#include "CRope.h"

void CRope::InjectHooks() {
    using namespace ReversibleHooks;
    Install("CRope", "ReleasePickedUpObject", 0x556030, &CRope::ReleasePickedUpObject);
    // Install("CRope", "CreateHookObjectForRope", 0x556070, &CRope::CreateHookObjectForRope);
    // Install("CRope", "UpdateWeightInRope", 0x5561B0, &CRope::UpdateWeightInRope);
    // Install("CRope", "Remove", 0x556780, &CRope::Remove);
    // Install("CRope", "Render", 0x556800, &CRope::Render);
    // Install("CRope", "PickUpObject", 0x5569C0, &CRope::PickUpObject);
    // Install("CRope", "Update", 0x557530, &CRope::Update);
}

// 0x556030
void CRope::ReleasePickedUpObject() {
    if (m_pRopeAttachObject) {
        m_pRopeAttachObject->physicalFlags.bAttachedToEntity = false;
        m_pRopeAttachObject->physicalFlags.b32 = false;
        m_pRopeAttachObject = 0;
    }
    m_pAttachedEntity->m_bUsesCollision = true;
    m_nFlags1 = 60; // 6th, 7th bits set
}

// 0x556070
void CRope::CreateHookObjectForRope() {
    plugin::CallMethod<0x556070, CRope*>(this);
}

// 0x5561B0
int8_t CRope::UpdateWeightInRope(float a2, float a3, float a4, int32 a5, float* a6) {
    return plugin::CallMethodAndReturn<int8_t, 0x5561B0, CRope*, float, float, float, int32, float*>(this, a2, a3, a4, a5, a6);
}

// 0x556780
void CRope::Remove() {
    plugin::CallMethod<0x556780, CRope*>(this);
}

// 0x556800
void CRope::Render() {
    plugin::CallMethod<0x556800, CRope*>(this);
}

// 0x5569C0
void CRope::PickUpObject(CEntity* a2) {
    plugin::CallMethod<0x5569C0, CRope*, CEntity*>(this, a2);
}

// 0x557530
void CRope::Update() {
    plugin::CallMethod<0x557530, CRope*>(this);
}
