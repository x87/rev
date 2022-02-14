#include "StdInc.h"

#include "EntryExit.h"
#include "EntryExitManager.h"
#include <optional>

bool& CEntryExit::ms_bWarping = *(bool*)0x96A7B8;
CObject*& CEntryExit::ms_pDoor = *(CObject**)0x96A7BC;
CEntryExit*& CEntryExit::ms_spawnPoint = *(CEntryExit**)0x96A7C0;

void CEntryExit::InjectHooks() {
    RH_ScopedClass(CEntryExit);
    RH_ScopedCategoryGlobal();

    RH_ScopedInstall(GenerateAmbientPeds, 0x43E8B0);
    RH_ScopedInstall(GetEntryExitToDisplayNameOf, 0x43E650);
    RH_ScopedInstall(GetPositionRelativeToOutsideWorld, 0x43EA00);
    RH_ScopedInstall(FindValidTeleportPoint, 0x43EAF0);
    RH_ScopedInstall(IsInArea, 0x43E460);
    // RH_ScopedInstall(TransitionStarted, 0x43FFD0);
    // RH_ScopedInstall(TransitionFinished, 0x4404A0);
    RH_ScopedInstall(RequestObjectsInFrustum, 0x43E690);
    RH_ScopedInstall(RequestAmbientPeds, 0x43E6D0);
    // RH_ScopedInstall(WarpGangWithPlayer, 0x43F1F0);
    // RH_ScopedInstall(ProcessStealableObjects, 0x43E990);
}

// 0x43E8B0
void CEntryExit::GenerateAmbientPeds(const CVector& posn) {
    CPopulation::bInPoliceStation = false;
    if (CGame::currArea != eAreaCodes::AREA_CODE_NORMAL_WORLD) {
        constexpr const char* PoliceStationEnExNames[]{ "POLICE1", "POLICE2", "POLICE3", "POLICE4" };
        CPopulation::bInPoliceStation =
            rng::any_of(PoliceStationEnExNames, [this](auto&& n) { return _stricmp(m_szName, n) == 0; });

        auto numPedsToSpawn = m_pLink ? m_pLink->m_nNumberOfPeds : m_nNumberOfPeds;
        CPopulation::NumberOfPedsInUseInterior = numPedsToSpawn;
        CPopulation::PopulateInterior(numPedsToSpawn, posn);
    } else {
        CPopulation::NumberOfPedsInUseInterior = 0;
        float exitAngle = DegreesToRadians(ms_spawnPoint->m_fExitAngle);
        CPopulation::CreateWaitingCoppers(posn, exitAngle);
    }
}

// 0x43E650
CEntryExit* CEntryExit::GetEntryExitToDisplayNameOf() {
    if (ms_spawnPoint->m_nArea != eAreaCodes::AREA_CODE_NORMAL_WORLD && HasNameSet()) {
        if (m_nArea != eAreaCodes::AREA_CODE_NORMAL_WORLD) {
            // TODO: Probably inlined from `CEntryExitManager`
            if (   CEntryExitManager::ms_entryExitStackPosn > 1
                && CEntryExitManager::ms_entryExitStack[CEntryExitManager::ms_entryExitStackPosn - 1] == ms_spawnPoint
            ) {
                return CEntryExitManager::ms_entryExitStack[CEntryExitManager::ms_entryExitStackPosn];
            }
            return this;
        }
    }
    return nullptr;
}

// 0x43EA00
void CEntryExit::GetPositionRelativeToOutsideWorld(CVector& outPos) {
    const auto enex = GetLinkedOrThis();
    if (enex->m_nArea != eAreaCodes::AREA_CODE_NORMAL_WORLD) {
        outPos += GetPosition() - enex->m_vecExitPos;
    }
}

// Return center of enterance rect
CVector CEntryExit::GetPosition() const {
    return CVector{ m_recEntrance.GetCenter(), m_fEntranceZ };
}

CVector2D CEntryExit::GetPosition2D() const {
    return CVector2D{ m_recEntrance.GetCenter() };
}

// Returns the matrix with which `rectEnterance`'s points were calculated.
CMatrix CEntryExit::GetRectEnteranceMatrix() const {
    CMatrix mat;
    mat.SetRotateZ(m_fEntranceAngle);
    mat.GetPosition() = GetPosition();
    return mat;
}

// Transforms a point into the enterance rect
CVector CEntryExit::TransformEnterancePoint(const CVector& point) const {
    return MultiplyMatrixWithVector(GetRectEnteranceMatrix(), point);
}

// 0x43EAF0
void CEntryExit::FindValidTeleportPoint(CVector* outTeleportPoint) {
    const auto spawnPointExitPos = ms_spawnPoint->m_vecExitPos;

    if (!CWorld::TestSphereAgainstWorld(spawnPointExitPos, 0.35f, nullptr, true, true, true, true, true, false)) {
        return;
    }

    // Test 8 spheres around the spawn point, and return whichever
    // doesn't collide with the world and the line of sight between it and `outPoint` is clear
    for (auto r : { 1.25f, 2.f }) { // Test with 2 ranges
        constexpr auto NumTestPoints{ 8 };
        for (auto i = 0; i < NumTestPoints; i++) {
            const auto rot{ (float)i * TWO_PI / (float)NumTestPoints };
            const auto point = *outTeleportPoint + CVector{
                std::cos(rot) * r,
                std::sin(rot) * r,
                0.f
            };
            if (!CWorld::TestSphereAgainstWorld(point, 0.35f, nullptr, true, true, true, true, true, false)) {
                if (CWorld::GetIsLineOfSightClear(*outTeleportPoint, point, true, true, false, true, true, false, false)) {
                    *outTeleportPoint = point;
                    return;
                }
            }
        }
    }
}

bool CEntryExit::HasNameSet() const {
    return m_szName[0] != 0;
}

// 0x43E460
bool CEntryExit::IsInArea(const CVector& position) {
    const auto CheckPointInRect = [this](const CVector& point) {
        if (m_recEntrance.IsPointInside(CVector2D{ point })) {
            if (std::abs(point.z - m_fEntranceZ) < 1.f) {
                return true;
            }
        }
        return false;
    };

    if (m_fEntranceAngle == 0.f) { // Common case whenn rotation is 0
        return CheckPointInRect(position);
    } else { // Sadly here we have to transform the point, and only then can we check if its in the rect
        return CheckPointInRect(MultiplyMatrixWithVector(GetRectEnteranceMatrix(), position));
    }
}

// 0x43FFD0
bool CEntryExit::TransitionStarted(CPed* ped) {
    return plugin::CallMethodAndReturn<bool, 0x43FFD0, CEntryExit*, CPed*>(this, ped);
}

// 0x4404A0
bool CEntryExit::TransitionFinished(CPed* ped) {
    return plugin::CallMethodAndReturn<bool, 0x4404A0, CEntryExit*, CPed*>(this, ped);
}

// 0x43E6D0
void CEntryExit::RequestAmbientPeds() {
    if (!CGame::CanSeeOutSideFromCurrArea()) {
        CStreaming::RemoveDodgyPedsFromRandomSlots();
        return;
    }

    if (_stricmp("bar1", m_szName) == 0) {
        int32 peds[] = {
            MODEL_BFYRI, MODEL_OFYST, MODEL_WFYST, MODEL_WMYST,
            MODEL_BMYRI, MODEL_BMYST, MODEL_OMOST, MODEL_OMYST,
        };
        CStreaming::StreamPedsIntoRandomSlots(peds);
    }

    if (_stricmp("strip2", m_szName) == 0) {
        int32 peds[] = {
            MODEL_SBFYSTR, MODEL_SWFYSTR, MODEL_INVALID, MODEL_INVALID,
            MODEL_INVALID, MODEL_INVALID, MODEL_INVALID, MODEL_INVALID,
        };
        CStreaming::StreamPedsIntoRandomSlots(peds);
    }

    if (_stricmp("LAstrip", m_szName) == 0) {
        int32 peds[] = {
            MODEL_VWFYST1, MODEL_VBFYST2, MODEL_VHFYST3, MODEL_INVALID,
            MODEL_INVALID, MODEL_INVALID, MODEL_INVALID, MODEL_INVALID,
        };
        CStreaming::StreamPedsIntoRandomSlots(peds);
    }

    if (_stricmp("MAFCAS", m_szName) == 0) {
        int32 peds[] = {
            MODEL_BFORI, MODEL_HFYRI, MODEL_OMYRI, MODEL_WMYRI,
            MODEL_OFYST, MODEL_VHMYELV, MODEL_WMOST, MODEL_BMORI,
        };
        CStreaming::StreamPedsIntoRandomSlots(peds);
    }

    if (_stricmp("TRICAS", m_szName) == 0) {
        int32 peds[] = {
            MODEL_BFYRI, MODEL_BMYRI, MODEL_HFORI, MODEL_HMORI,
            MODEL_WMORI, MODEL_WFYRI, MODEL_OMOST, MODEL_VBMYELV,
        };
        CStreaming::StreamPedsIntoRandomSlots(peds);
    }
}
// 0x43E690
void CEntryExit::RequestObjectsInFrustum() const {
    CRenderer::RequestObjectsInDirection(ms_spawnPoint->m_vecExitPos, m_fExitAngle, 0);
}

// 0x43F1F0
void CEntryExit::WarpGangWithPlayer(CPed* ped) {
    plugin::CallMethod<0x43F1F0, CEntryExit*, CPed*>(this, ped);
}

// 0x43E990
void CEntryExit::ProcessStealableObjects(CPed* ped) {
    plugin::CallMethod<0x43E990, CEntryExit*, CPed*>(this, ped);
}
