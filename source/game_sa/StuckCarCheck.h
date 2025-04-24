/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Vehicle.h"
#include "Vector.h"

#define STUCK_CAR_CHECK_SIZE 16

struct tStuckCar {
    int32   m_nCarHandle;
    CVector m_vCarPos;
    int32   m_nStartTime;
    float   m_fDistance;
    int32   m_nStuckTime;
    bool    m_bCarStuck;
    char    m_bWarpCar;
    bool    m_bStuck;
    bool    m_bFlipped;
    bool    m_bInWater;
    int8    m_nNumberOfNodesToCheck;
};

class CStuckCarCheck {
public:
    std::array<tStuckCar, STUCK_CAR_CHECK_SIZE> m_aStuckCars;

    static void InjectHooks();

    void Init();
    void AddCarToCheck(int32 carHandle, float distance, uint32 time, uint8 bWarpCar, bool bStuck, bool bFlipped, bool bInWater, int8 numberOfNodesToCheck);
    bool AttemptToWarpVehicle(CVehicle* vehicle, CVector* origin, float orientation);
    void ClearStuckFlagForCar(int32 carHandle);
    bool HasCarBeenStuckForAWhile(int32 carHandle);
    bool IsCarInStuckCarArray(int32 carHandle);
    void Process();
    void RemoveCarFromCheck(int32 carHandle);
    void ResetArrayElement(uint16 carHandle);
    void ResetArrayElement(tStuckCar& car);
};

VALIDATE_SIZE(CStuckCarCheck, 0x240);
