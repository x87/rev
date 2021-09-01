#include "StdInc.h"
#include "CRope.h"

void CRope::InjectHooks() {
    // ReversibleHooks::Install("CRope", "ReleasePickedUpObject", 0x556030, &CRope::ReleasePickedUpObject);
    // ReversibleHooks::Install("CRope", "CreateHookObjectForRope", 0x556070, &CRope::CreateHookObjectForRope);
    // ReversibleHooks::Install("CRope", "UpdateWeightInRope", 0x5561B0, &CRope::UpdateWeightInRope);
    // ReversibleHooks::Install("CRope", "Remove", 0x556780, &CRope::Remove);
    // ReversibleHooks::Install("CRope", "Render", 0x556800, &CRope::Render);
    // ReversibleHooks::Install("CRope", "PickUpObject", 0x5569C0, &CRope::PickUpObject);
    // ReversibleHooks::Install("CRope", "Update", 0x557530, &CRope::Update);
}

// 0x556030
int32_t CRope::ReleasePickedUpObject() {
    return plugin::CallMethodAndReturn<int32, 0x556030, CRope*>(this);
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
