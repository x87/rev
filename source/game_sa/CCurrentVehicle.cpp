#include "StdInc.h"

#include "CCurrentVehicle.h"

void CCurrentVehicle::InjectHooks() {
    ReversibleHooks::Install("CCurrentVehicle", "Display", 0x571EA0, &CCurrentVehicle::Display);
    ReversibleHooks::Install("CCurrentVehicle", "Process", 0x572040, &CCurrentVehicle::Process);
}

CCurrentVehicle::CCurrentVehicle() {
    Init();
}

void CCurrentVehicle::Init() {
    vehicle = nullptr;
}

// 0x571EA0
void CCurrentVehicle::Display() const {
    char* name = nullptr;
    if (vehicle) {
        auto modelInfo = static_cast<CVehicleModelInfo*>(CModelInfo::ms_modelInfoPtrs[vehicle->m_nModelIndex]);
        auto gameName = modelInfo->m_szGameName;
        name = TheText.Get(gameName);
    }
    CHud::SetVehicleName(name);
}

// 0x572040
void CCurrentVehicle::Process() {
    auto player = FindPlayerPed();
    if (player->bInVehicle) {
        vehicle = player->m_pVehicle;
    } else {
        vehicle = nullptr;
    }

    Display();
}
