#include "StdInc.h"
#include "TaskComplexCopInCar.h"
#include "TaskSimpleCarDrive.h"
#include "TaskComplexLeaveCar.h"
#include "TaskComplexEnterCarAsDriver.h"
#include "TaskSimpleStandStill.h"
#include "TaskComplexEnterCarAsPassenger.h"
#include "TaskComplexPolicePursuit.h"
#include "TaskComplexWanderCop.h"
#include "TaskComplexCarDrive.h"
#include "TaskComplexCarDriveMission.h"
#include "Enums/eCarDrivingStyle.h"

void CTaskComplexCopInCar::InjectHooks() {
    RH_ScopedClass(CTaskComplexCopInCar);
    RH_ScopedCategory("Tasks/TaskTypes");

    RH_ScopedInstall(Constructor, 0x68C7F0);
    RH_ScopedInstall(Destructor, 0x68C8C0);

    RH_ScopedInstall(CreateSubTask, 0x68C9E0);
    RH_ScopedInstall(Clone_Reversed, 0x68CEC0);
    RH_ScopedInstall(GetTaskType_Reversed, 0x68C8B0);
    RH_ScopedInstall(MakeAbortable_Reversed, 0x68C940);
    RH_ScopedInstall(CreateNextSubTask_Reversed, 0x68FA50);
    // RH_ScopedInstall(CreateFirstSubTask_Reversed, 0x68FA10);
    // RH_ScopedInstall(ControlSubTask_Reversed, 0x68FD50);
}

// 0x68C7F0
CTaskComplexCopInCar::CTaskComplexCopInCar(CVehicle* vehicle, CPed* cop1, CPed* cop2, bool flag_0x1) :
    m_pVehicle{vehicle},
    m_pCop1{cop1},
    m_pCop2{cop2},
    m_flag0x1{flag_0x1}
{
    CEntity::SafeRegisterRef(m_pVehicle);
    CEntity::SafeRegisterRef(m_pCop1);
    CEntity::SafeRegisterRef(m_pCop2);
}

// NOTSA
CTaskComplexCopInCar::CTaskComplexCopInCar(const CTaskComplexCopInCar& o) :
    CTaskComplexCopInCar{o.m_pVehicle, o.m_pCop1, o.m_pCop2, o.m_flag0x1}
{
}

// 0x68C8C0
CTaskComplexCopInCar::~CTaskComplexCopInCar() {
    CEntity::SafeCleanUpRef(m_pVehicle);
    CEntity::SafeCleanUpRef(m_pCop1);
    CEntity::SafeCleanUpRef(m_pCop2);
}
 
// 0x68C9E0
CTask* CTaskComplexCopInCar::CreateSubTask(eTaskType taskType, CPed* copPed) {
    switch (taskType) {
    case TASK_SIMPLE_CAR_DRIVE:
        return new CTaskSimpleCarDrive{ m_pVehicle };
    case TASK_COMPLEX_LEAVE_CAR: {
        copPed->GetIntelligence()->SetPedDecisionMakerType(DM_EVENT_KNOCK_OFF_BIKE);
        return new CTaskComplexLeaveCar{ m_pVehicle, 0, 0, true, false };
    }
    case TASK_COMPLEX_ENTER_CAR_AS_DRIVER: {
        copPed->GetIntelligence()->SetPedDecisionMakerType(DM_EVENT_SHOT_FIRED);
        return new CTaskComplexEnterCarAsDriver{ m_pVehicle };
    }
    case TASK_SIMPLE_STAND_STILL:
        return new CTaskSimpleStandStill{ 1000, true, false, 8.f };
    case TASK_COMPLEX_ENTER_CAR_AS_PASSENGER: {
        copPed->GetIntelligence()->SetPedDecisionMakerType(DM_EVENT_SHOT_FIRED);
        return new CTaskComplexEnterCarAsPassenger{ m_pVehicle, 0, false };
    }
    case TASK_COMPLEX_POLICE_PURSUIT:
        return new CTaskComplexPolicePursuit{};
    case TASK_COMPLEX_WANDER:
        return new CTaskComplexWanderCop{ PEDMOVE_WALK, (uint8)CGeneral::GetRandomNumberInRange(0, 8) };
    case TASK_COMPLEX_CAR_DRIVE_MISSION: {
        const auto targetEntity = m_pCop2->bInVehicle ? (CEntity*)m_pCop2->m_pVehicle : (CEntity*)m_pCop2;
        if (m_pCop2->IsPlayer()) {
            return new CTaskComplexCarDriveMission{
                m_pVehicle,
                targetEntity,
                m_pCop2->bInVehicle ? (eCarMission)CCarAI::FindPoliceCarMissionForWantedLevel() : MISSION_POLICE_BIKE,
                (eCarDrivingStyle)CCarAI::FindPoliceCarSpeedForWantedLevel(m_pVehicle), // TODO: This really doesn't add up.. How does this work?
                10.f
            };
        } else {
            const auto GetDrivingMission = [this] {
                if (m_pCop2->bInVehicle) {
                    return rand() % 4 < 2 ? MISSION_BLOCKPLAYER_FARAWAY : MISSION_RAMPLAYER_FARAWAY;
                }
                return MISSION_37;
            };
            return new CTaskComplexCarDriveMission{
                m_pVehicle,
                targetEntity,
                GetDrivingMission(),
                (eCarDrivingStyle)(this->m_pVehicle->m_pHandlingData->m_transmissionData.field_5C * 54.0), // TODO: This really doesn't add up.. How does this work?
                10.f
            };
        }
    }
    case TASK_FINISHED:
        return nullptr;

    default:
        NOTSA_UNREACHABLE;
        return nullptr;
    }
}

// 0x68C940
bool CTaskComplexCopInCar::MakeAbortable(CPed* ped, eAbortPriority priority, CEvent const* event) {
    if (!m_pSubTask) {
        return true;
    }

    if (!m_pSubTask->MakeAbortable(ped, priority, event)) {
        return false;
    }

    if (event && event->GetEventType() == EVENT_DAMAGE) {
        const auto dmgEvent = (CEventDamage*)event;
        if (dmgEvent->m_damageResponse.m_bHealthZero) {
            if (dmgEvent->m_bAddToEventGroup && ped->bInVehicle) {
                if (ped->m_pVehicle == m_pVehicle && m_pVehicle) {
                    if (m_pVehicle->IsDriver(ped)) {
                        m_pVehicle->m_nStatus = STATUS_ABANDONED;
                        m_pVehicle->m_autoPilot.m_nCarMission = MISSION_NONE;
                        m_pVehicle->m_autoPilot.m_nCruiseSpeed = 0;
                    }
                }
            }
        }
    }

    return true;
}

// 0x68FA50
CTask* CTaskComplexCopInCar::CreateNextSubTask(CPed* ped) {
    // Ped is a CopPed*

    if (!m_pCop2 || m_pCop2->IsStateDead()) {
        return CreateSubTask(TASK_FINISHED, ped);
    }

    switch (m_pSubTask->GetTaskType()) {
    case TASK_COMPLEX_CAR_DRIVE_MISSION: {
        m_flag0x2 = true;
        return CreateSubTask(TASK_SIMPLE_CAR_DRIVE, ped);
    }
    case TASK_COMPLEX_POLICE_PURSUIT: {
        if (!FindPlayerWanted()->m_nWantedLevel) {
            return CreateSubTask(TASK_FINISHED, ped);
        }

        if (FindPlayerWanted()->CanCopJoinPursuit((CCopPed*)ped) && static_cast<CTaskComplexPolicePursuit*>(m_pSubTask)->m_nFlags & 4) {
            // 0x68FBC6 - Inverted
            if (m_pCop2->bIsBeingArrested) {
                return CreateSubTask(TASK_FINISHED, ped);
            }

            if (m_pCop2->bInVehicle && m_pVehicle != m_pCop2->m_pVehicle) {
                if (m_pVehicle && m_flag0x1) {
                    if (ped->bInVehicle) {
                        return CreateSubTask(TASK_SIMPLE_CAR_DRIVE, ped);
                    } else if ((m_pVehicle->GetPosition() - ped->GetPosition()).SquaredMagnitude() < 4.f * 4.f) {
                        return CreateSubTask(TASK_COMPLEX_ENTER_CAR_AS_DRIVER, ped);
                    }
                }
            }
        }

        m_timer2.m_nStartTime = CTimer::m_snTimeInMilliseconds;
        m_timer2.m_nInterval = 3000;
        m_timer2.m_bStarted = 1;
        return CreateSubTask(TASK_COMPLEX_WANDER, ped);
    }
    case TASK_SIMPLE_CAR_DRIVE: {
        m_pVehicle->m_autoPilot.m_nCarMission = MISSION_NONE;
        return CreateSubTask(TASK_COMPLEX_LEAVE_CAR, ped);
    }
    case TASK_COMPLEX_ENTER_CAR_AS_PASSENGER: // 0x68FA89
    case TASK_COMPLEX_ENTER_CAR_AS_DRIVER: // 0x68FA94
    {
        if (!ped->bInVehicle || !ped->m_pVehicle) { // TODO: Invert this, and use ped->IsInVehicle()
            return CreateSubTask(TASK_COMPLEX_POLICE_PURSUIT, ped);
        } else {
            ped->m_pVehicle->ChangeLawEnforcerState(true);
            m_flag0x4 = false;
            m_flag0x8 = ped->bIsLanding; // TODO: Not sure.
            return CreateSubTask(TASK_COMPLEX_CAR_DRIVE_MISSION, ped);
        }
    }
    case TASK_COMPLEX_LEAVE_CAR: { // 0x68FAA0
        return CreateSubTask(TASK_COMPLEX_POLICE_PURSUIT, ped);
    }
    default:
        return CreateSubTask(TASK_FINISHED, ped);
    }
}

// 0x68FA10
CTask* CTaskComplexCopInCar::CreateFirstSubTask(CPed* ped) {
    return plugin::CallMethodAndReturn<CTask*, 0x68FA10, CTaskComplexCopInCar*, CPed*>(this, ped);
}

// 0x68FD50
CTask* CTaskComplexCopInCar::ControlSubTask(CPed* ped) {
    return plugin::CallMethodAndReturn<CTask*, 0x68FD50, CTaskComplexCopInCar*, CPed*>(this, ped);
}
