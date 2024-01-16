#include "StdInc.h"
#include "TaskSimpleOnEscalator.h"

void CTaskSimpleOnEscalator::InjectHooks() {
    RH_ScopedVirtualClass(CTaskSimpleOnEscalator, 0x85b8dc, 9);
    RH_ScopedCategory("Tasks/TaskTypes");

    RH_ScopedInstall(Constructor, 0x4B8670);
    RH_ScopedInstall(Destructor, 0x4B8750);

    RH_ScopedVMTInstall(Clone, 0x4B86C0);
    RH_ScopedVMTInstall(GetTaskType, 0x4B8720);
    RH_ScopedVMTInstall(ProcessPed, 0x62F520);
}

// 0x4B8670
CTaskSimpleOnEscalator::CTaskSimpleOnEscalator() :
    CTaskSimpleStandStill{0, true, false, 8.0f}
{
}

// 0x4B86C0
CTaskSimpleOnEscalator::CTaskSimpleOnEscalator(const CTaskSimpleOnEscalator&) :
    CTaskSimpleOnEscalator{}
{
}

// 0x62F520
bool CTaskSimpleOnEscalator::ProcessPed(CPed* ped) {
    CTaskSimpleStandStill::ProcessPed(ped);
    if (!ped->m_pContactEntity) {
        return true;
    }
    const auto escl = ped->m_pContactEntity->AsPhysical();
    assert(escl->IsObject());
    if (!notsa::contains<eModelID>({ ModelIndices::MI_ESCALATORSTEP, ModelIndices::MI_ESCALATORSTEP8 }, ped->GetModelID())) {
        return true;
    }
    ped->m_fAimingRotation = escl->GetMoveSpeed().Heading();
    return false;
}
