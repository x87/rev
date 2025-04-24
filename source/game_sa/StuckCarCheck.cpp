#include "StdInc.h"

#include "StuckCarCheck.h"

void CStuckCarCheck::InjectHooks() {
    RH_ScopedClass(CStuckCarCheck);
    RH_ScopedCategoryGlobal();

    RH_ScopedInstall(Init, 0x4639E0);
    RH_ScopedInstall(AddCarToCheck, 0x465970);
    RH_ScopedInstall(AttemptToWarpVehicle, 0x463A60);
    RH_ScopedInstall(ClearStuckFlagForCar, 0x463C40);
    RH_ScopedInstall(HasCarBeenStuckForAWhile, 0x463C00);
    RH_ScopedInstall(IsCarInStuckCarArray, 0x463C70);
    RH_ScopedInstall(Process, 0x465680);
    RH_ScopedInstall(RemoveCarFromCheck, 0x463B80);
}

// 0x4639E0
void CStuckCarCheck::Init() {
    for (auto& car : m_aStuckCars) {
        ResetArrayElement(car);
    }
}

// 0x465970
void CStuckCarCheck::AddCarToCheck(int32 carHandle, float distance, uint32 time, uint8 bWarpCar, bool bStuck, bool bFlipped, bool bInWater, int8 numberOfNodesToCheck) {
    const auto vehicle = CPools::GetVehicle(carHandle);

    if (!vehicle) {
        return;
    }

    for (auto& car : m_aStuckCars) {
        // NOTSA optimization
        if (car.m_nCarHandle == -1) {
            car.m_nCarHandle            = carHandle;
            car.m_vCarPos               = vehicle->GetPosition();
            car.m_nStartTime            = CTimer::m_snTimeInMilliseconds;
            car.m_fDistance             = distance;
            car.m_nStuckTime            = time;
            car.m_bWarpCar              = bWarpCar;
            car.m_bStuck                = bStuck;
            car.m_bFlipped              = bFlipped;
            car.m_bCarStuck             = false;
            car.m_bInWater              = bInWater;
            car.m_nNumberOfNodesToCheck = numberOfNodesToCheck;
            return;
        }
    }
}

// 0x463A60
bool CStuckCarCheck::AttemptToWarpVehicle(CVehicle* vehicle, CVector* origin, float orientation) {
    if (TheCamera.IsSphereVisible(*origin, 4.0f)) {
        return false;
    }

    CVector min = *origin - CVector(4.0f, 4.0f, 4.0f);
    CVector max = *origin + CVector(4.0f, 4.0f, 4.0f);

    int16 outCount = {};
    CWorld::FindMissionEntitiesIntersectingCube(min, max, &outCount, 2, nullptr, true, true, true);

    if (outCount) {
        return false;
    }

    CCarCtrl::SetCoordsOfScriptCar(vehicle, origin->x, origin->y, origin->z, 1, 1);
    vehicle->SetHeading(DegreesToRadians(orientation));
    return true;
}

// 0x463C40
void CStuckCarCheck::ClearStuckFlagForCar(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_nCarHandle == carHandle) {
            car.m_bCarStuck = false;
        }
    }
}

// 0x463C00
bool CStuckCarCheck::HasCarBeenStuckForAWhile(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_nCarHandle == carHandle && car.m_bCarStuck) {
            return true;
        }
    }
    return false;
}

// 0x463C70
bool CStuckCarCheck::IsCarInStuckCarArray(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_nCarHandle == carHandle) {
            return true;
        }
    }
    return false;
}

// 0x465680
void CStuckCarCheck::Process() {
    ZoneScoped;

    for (auto& car : m_aStuckCars) {
        if (car.m_nCarHandle >= 0) {
            auto vehicle = CPools::GetVehicle(car.m_nCarHandle);
            if (vehicle) {
                if (vehicle->m_pDriver) {
                    if (CTimer::m_snTimeInMilliseconds > (uint32)car.m_nStartTime + car.m_nStuckTime) {
                        CVector curPos    = vehicle->GetPosition();
                        CVector distMoved = curPos - car.m_vCarPos;

                        car.m_bCarStuck  = distMoved.Magnitude() < car.m_fDistance;
                        car.m_vCarPos    = curPos;
                        car.m_nStartTime = CTimer::m_snTimeInMilliseconds;
                    }
                    if (car.m_bWarpCar) {
                        bool shouldWarp = false;
                        if (car.m_bCarStuck && car.m_bStuck) {
                            shouldWarp = true;
                        }
                        if (car.m_bFlipped && CTheScripts::UpsideDownCars.IsCarUpsideDown(vehicle)) {
                            shouldWarp = true;
                        }
                        if (car.m_bInWater && vehicle->physicalFlags.bSubmergedInWater) {
                            shouldWarp = true;
                        }
                        if (shouldWarp) {
                            float   radius      = vehicle->GetModelInfo()->GetColModel()->GetBoundRadius();
                            CVector boundCentre = vehicle->GetBoundCentre();
                            if (!TheCamera.IsSphereVisible(boundCentre, radius)) {
                                if (car.m_nNumberOfNodesToCheck >= 0) {
                                    for (int32 i = 0; i < car.m_nNumberOfNodesToCheck; i++) { // m_nNumberOfNodesToCheck 0 is ignored
                                        CVector newCoords;
                                        auto    node = ThePaths.FindNthNodeClosestToCoors(car.m_vCarPos, PATH_TYPE_VEH, 999999.88f, false, true, i, false, false, nullptr);
                                        if (ThePaths.FindNodeCoorsForScript(newCoords, node)) {
                                            auto newHeading = ThePaths.FindNodeOrientationForCarPlacement(node);
                                            if (CStuckCarCheck::AttemptToWarpVehicle(vehicle, &newCoords, newHeading)) {
                                                break;
                                            }
                                        }
                                    }
                                } else {
                                    bool    found      = {};
                                    float   newHeading = {};
                                    CVector newCoords  = ThePaths.FindNodeCoorsForScript(vehicle->m_autoPilot.m_startingRouteNode, vehicle->m_autoPilot.m_endingRouteNode, newHeading, &found);
                                    if (found) {
                                        CStuckCarCheck::AttemptToWarpVehicle(vehicle, &newCoords, newHeading);
                                    }
                                }
                            }
                        }
                    } else {
                        car.m_vCarPos  = vehicle->GetPosition();
                        car.m_nStartTime = CTimer::m_snTimeInMilliseconds;
                    }
                } else {
                    ResetArrayElement(car);
                }
            }
        }
    }
}

// 0x463B80
void CStuckCarCheck::RemoveCarFromCheck(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_nCarHandle == carHandle) {
            ResetArrayElement(car);
        }
    }
}

// 0x463970 | refactored
void CStuckCarCheck::ResetArrayElement(uint16 carHandle) {
    ResetArrayElement(m_aStuckCars[carHandle]);
}

// NOTSA
void CStuckCarCheck::ResetArrayElement(tStuckCar& car) {
    car.m_vCarPos               = CVector(-5000.0f, -5000.0f, -5000.0f);
    car.m_nCarHandle            = -1;
    car.m_nStartTime            = -1;
    car.m_fDistance             = 0.0f;
    car.m_nStuckTime            = 0;
    car.m_bCarStuck             = false;
    car.m_bWarpCar              = false;
    car.m_bStuck                = false;
    car.m_bFlipped              = false;
    car.m_bInWater              = false;
    car.m_nNumberOfNodesToCheck = 0;
}
