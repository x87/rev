#pragma once

#include "TaskComplex.h"
#include "Vector.h"
#include "Base.h"

class CEntity;

class CTaskComplexSmartFleeEntity : public CTaskComplex {
public:
    CEntity*   m_entity{};                      /// Entity to flee from
    CVector    m_pos{};                         /// The current position to flee from (It's the entity's position with some tolerance, see `m_posCheckTimer`)
    int32      m_time{};                        /// The time in which we have to flee
    float      m_safeDistance{};                /// Distance from entity in which we're safe (thus task ends)
    bool       m_pedScream{};                   /// Should the ped scream (Using `CPed::Say`)
    int32      m_posCheckPeriod{};              /// How often the entity's position should be checked
    float      m_posChangeTolerance{};          /// How much of a tolerance there is for the entity's position (presumeably in distance from the prev. pos) (eg.: anything less than this will be ignored)
    eMoveState m_moveState{ PEDMOVE_SPRINT };   /// The ped's move state when fleeing
    CTaskTimer m_posCheckTimer{};               /// Entity's position check timer (Period is `m_posCheckPeriod`)

public:
    static constexpr auto Type = TASK_COMPLEX_SMART_FLEE_ENTITY;

    /*!
    * @param fleeEntity @copybrief m_entity
    * @param scream @copybrief m_pedScream
    * @param safeDistance @copybrief m_safeDistance
    * @param fleeTime @copybrief m_time
    * @param posCheckPeriod @copybrief m_posCheckPeriod
    * @param posChangeTolerance @copybrief m_posChangeTolerance
    */
    CTaskComplexSmartFleeEntity(
        CEntity* fleeEntity,
        bool scream,
        float safeDistance,
        int32 fleeTime,
        int32 posCheckPeriod,
        float posChangeTolerance
    );
    CTaskComplexSmartFleeEntity(const CTaskComplexSmartFleeEntity&);
    ~CTaskComplexSmartFleeEntity() override;

    eTaskType GetTaskType() override { return Type; } // 0x65C4C0
    CTask*    Clone() override { return new CTaskComplexSmartFleeEntity{ *this }; }
    CTask*    CreateNextSubTask(CPed* ped) override;
    CTask*    CreateFirstSubTask(CPed* ped) override;
    CTask*    ControlSubTask(CPed* ped) override;
    CTask*    CreateSubTask(eTaskType taskType);

private:
    friend void InjectHooksMain();
    static void InjectHooks();
    CTaskComplexSmartFleeEntity* Constructor(CEntity* entity_1, bool a3, float a4, int32 a5, int32 a6, float a7) { this->CTaskComplexSmartFleeEntity::CTaskComplexSmartFleeEntity(entity_1, a3, a4, a5, a6, a7); return this; }
    CTaskComplexSmartFleeEntity* Destructor() { this->CTaskComplexSmartFleeEntity::~CTaskComplexSmartFleeEntity(); return this; }
};
VALIDATE_SIZE(CTaskComplexSmartFleeEntity, 0x40);
