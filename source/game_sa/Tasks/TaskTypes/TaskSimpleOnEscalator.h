#pragma once

#include "TaskSimpleStandStill.h"

class CPed;
class CTaskSimpleOnEscalator;

class NOTSA_EXPORT_VTABLE CTaskSimpleOnEscalator final : public CTaskSimpleStandStill {
public:
    static constexpr auto Type = eTaskType::TASK_SIMPLE_ON_ESCALATOR;

    static void InjectHooks();

    CTaskSimpleOnEscalator();
    CTaskSimpleOnEscalator(const CTaskSimpleOnEscalator&);
    ~CTaskSimpleOnEscalator() override = default;

    CTask*    Clone() const override { return new CTaskSimpleOnEscalator{ *this }; }
    eTaskType GetTaskType() const override { return Type; }
    bool      ProcessPed(CPed* ped) override;

private: // Wrappers for hooks
    // 0x4B8670
    CTaskSimpleOnEscalator* Constructor() {
        this->CTaskSimpleOnEscalator::CTaskSimpleOnEscalator();
        return this;
    }

    // 0x4B8750
    CTaskSimpleOnEscalator* Destructor() {
        this->CTaskSimpleOnEscalator::~CTaskSimpleOnEscalator();
        return this;
    }
};
