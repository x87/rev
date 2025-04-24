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
void CStuckCarCheck::AddCarToCheck(int32 carHandle, float stuckRadius, uint32 time, bool warpCar, bool stuck, bool flipped, bool inWater, int8 numberOfNodesToCheck) {
    const auto vehicle = CPools::GetVehicle(carHandle);

    if (!vehicle) {
        return;
    }

    for (auto& car : m_aStuckCars) {
        // NOTSA optimization
        if (car.m_CarHandle == -1) {
            car.m_CarHandle            = carHandle;
            car.m_CarPos               = vehicle->GetPosition();
            car.m_LastChecked          = CTimer::m_snTimeInMilliseconds;
            car.m_StuckRadius          = stuckRadius;
            car.m_CheckTime            = time;
            car.m_WarpCar              = warpCar;
            car.m_WarpIfStuck          = stuck;
            car.m_WarpIfFlipped        = flipped;
            car.m_CarStuck             = false;
            car.m_WarpIfInWater        = inWater;
            car.m_NumberOfNodesToCheck = numberOfNodesToCheck;
            return;
        }
    }
}

// 0x463A60
bool CStuckCarCheck::AttemptToWarpVehicle(CVehicle* vehicle, CVector* origin, float orientation) {
    if (TheCamera.IsSphereVisible(*origin, 4.0f)) {
        return false;
    }

    int16   outCount = {};
    CVector offset{ 4.0f, 4.0f, 4.0f };
    CWorld::FindMissionEntitiesIntersectingCube(*origin - offset, *origin + offset, &outCount, 2, nullptr, true, true, true);

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
        if (car.m_CarHandle == carHandle) {
            car.m_CarStuck = false;
        }
    }
}

// 0x463C00
bool CStuckCarCheck::HasCarBeenStuckForAWhile(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_CarHandle == carHandle && car.m_CarStuck) {
            return true;
        }
    }
    return false;
}

// 0x463C70
bool CStuckCarCheck::IsCarInStuckCarArray(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_CarHandle == carHandle) {
            return true;
        }
    }
    return false;
}

// 0x465680
void CStuckCarCheck::Process() {
    ZoneScoped;

    for (auto& car : m_aStuckCars) {
        if (car.m_CarHandle < 0) {
            continue;
        }

        auto vehicle = CPools::GetVehicle(car.m_CarHandle);

        if (!vehicle) {
            ResetArrayElement(car);
            continue;
        }

        if (!vehicle->m_pDriver) {
            car.m_CarPos      = vehicle->GetPosition();
            car.m_LastChecked = CTimer::m_snTimeInMilliseconds;
            continue;
        }

        if (CTimer::m_snTimeInMilliseconds > (uint32)car.m_LastChecked + car.m_CheckTime) {
            CVector curPos    = vehicle->GetPosition();
            CVector distMoved = curPos - car.m_CarPos;

            car.m_CarStuck    = distMoved.Magnitude() < car.m_StuckRadius;
            car.m_CarPos      = curPos;
            car.m_LastChecked = CTimer::m_snTimeInMilliseconds;
        }

        if (!car.m_WarpCar) {
            continue;
        }

        bool shouldWarp = false;
        if (car.m_WarpIfStuck && car.m_CarStuck) {
            shouldWarp = true;
        }
        if (car.m_WarpIfFlipped && CTheScripts::UpsideDownCars.IsCarUpsideDown(vehicle)) {
            shouldWarp = true;
        }
        if (car.m_WarpIfInWater && vehicle->physicalFlags.bSubmergedInWater) {
            shouldWarp = true;
        }

        if (!shouldWarp) {
            continue;
        }

        if (TheCamera.IsSphereVisible(vehicle->GetBoundCentre(), vehicle->GetModelInfo()->GetColModel()->GetBoundRadius())) {
            continue;
        }

        if (car.m_NumberOfNodesToCheck < 0) {
            bool    found{};
            float   newHeading{};
            CVector newCoords = ThePaths.FindNodeCoorsForScript(vehicle->m_autoPilot.m_currentAddress, vehicle->m_autoPilot.m_startingRouteNode, newHeading, &found);
            if (found) {
                CStuckCarCheck::AttemptToWarpVehicle(vehicle, &newCoords, newHeading);
            }
            continue;
        }

        for (int32 i = 0; i < car.m_NumberOfNodesToCheck; i++) { // m_nNumberOfNodesToCheck 0 is ignored
            CVector newCoords{};
            auto    node = ThePaths.FindNthNodeClosestToCoors(car.m_CarPos, PATH_TYPE_VEH, 999999.88f, false, true, i, false, false, nullptr);
            if (ThePaths.FindNodeCoorsForScript(newCoords, node)) {
                auto newHeading = ThePaths.FindNodeOrientationForCarPlacement(node);
                if (CStuckCarCheck::AttemptToWarpVehicle(vehicle, &newCoords, newHeading)) {
                    break;
                }
            }
        }
    }
}

// 0x463B80
void CStuckCarCheck::RemoveCarFromCheck(int32 carHandle) {
    for (auto& car : m_aStuckCars) {
        if (car.m_CarHandle == carHandle) {
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
    car.m_CarPos               = CVector(-5000.0f, -5000.0f, -5000.0f);
    car.m_CarHandle            = -1;
    car.m_LastChecked          = -1;
    car.m_StuckRadius          = 0.0f;
    car.m_CheckTime            = 0;
    car.m_CarStuck             = false;
    car.m_WarpCar              = false;
    car.m_WarpIfStuck          = false;
    car.m_WarpIfFlipped        = false;
    car.m_WarpIfInWater        = false;
    car.m_NumberOfNodesToCheck = 0;
}
