#include "StdInc.h"
#include "GenericGameStorage.h"
#include <SimpleVariablesSaveStructure.h>
#include <TheCarGenerators.h>
#include <PedType.h>

#include <ranges>
namespace rng = std::ranges;

void CGenericGameStorage::InjectHooks() {
    using namespace ReversibleHooks;
    // Static functions (19x)
    Install("CGenericGameStorage", "ReportError", 0x5D08C0, &CGenericGameStorage::ReportError);
    Install("CGenericGameStorage", "DoGameSpecificStuffAfterSucessLoad", 0x618E90, &CGenericGameStorage::DoGameSpecificStuffAfterSucessLoad);
    Install("CGenericGameStorage", "InitRadioStationPositionList", 0x618E70, &CGenericGameStorage::InitRadioStationPositionList);
    Install("CGenericGameStorage", "GetSavedGameDateAndTime", 0x618D00, &CGenericGameStorage::GetSavedGameDateAndTime);
    Install("CGenericGameStorage", "GenericLoad", 0x5D17B0, &CGenericGameStorage::GenericLoad);
    Install("CGenericGameStorage", "GenericSave", 0x5D13E0, &CGenericGameStorage::GenericSave);
    // Install("CGenericGameStorage", "CheckSlotDataValid", 0x5D1380, &CGenericGameStorage::CheckSlotDataValid);
    // Install("CGenericGameStorage", "LoadDataFromWorkBuffer", 0x5D1300, &CGenericGameStorage::LoadDataFromWorkBuffer);
    // Install("CGenericGameStorage", "DoGameSpecificStuffBeforeSave", 0x618F50, &CGenericGameStorage::DoGameSpecificStuffBeforeSave);
    // Install("CGenericGameStorage", "SaveDataToWorkBuffer", 0x5D1270, &CGenericGameStorage::SaveDataToWorkBuffer);
    // Install("CGenericGameStorage", "LoadWorkBuffer", 0x5D10B0, &CGenericGameStorage::LoadWorkBuffer);
    // Install("CGenericGameStorage", "SaveWorkBuffer", 0x5D0F80, &CGenericGameStorage::SaveWorkBuffer);
    // Install("CGenericGameStorage", "GetCurrentVersionNumber", 0x5D0F50, &CGenericGameStorage::GetCurrentVersionNumber);
    // Install("CGenericGameStorage", "MakeValidSaveName", 0x5D0E90, &CGenericGameStorage::MakeValidSaveName);
    // Install("CGenericGameStorage", "CloseFile", 0x5D0E30, &CGenericGameStorage::CloseFile);
    // Install("CGenericGameStorage", "OpenFileForWriting", 0x5D0DD0, &CGenericGameStorage::OpenFileForWriting_void_);
    // Install("CGenericGameStorage", "OpenFileForReading", 0x5D0D20, &CGenericGameStorage::OpenFileForReading);
    // Install("CGenericGameStorage", "CheckDataNotCorrupt", 0x5D1170, &CGenericGameStorage::CheckDataNotCorrupt);
    // Install("CGenericGameStorage", "RestoreForStartLoad", 0x619000, &CGenericGameStorage::RestoreForStartLoad);
}

// Static functions
// 0x5D08C0
void CGenericGameStorage::ReportError(eBlocks nBlock, eSaveLoadError nError) {
    char buffer[256]{};
    const auto GetErrorString = [nError] {
        switch (nError) {
        case eSaveLoadError::LOADING:
            return "Loading error: %s";
        case eSaveLoadError::SAVING:
            return "Saving error: %s";
        case eSaveLoadError::SYNC:
            return "Loading sync error: %s";
        default:
            return "Unknown error: %s";
        }
    };
    sprintf_s(buffer, GetErrorString(), GetBlockName(nBlock));

    // Yes, they don't do anything with `buffer`

    std::cerr << "[CGenericGameStorage]: " << buffer << std::endl; // NOTSA
}

// 0x618E90
void CGenericGameStorage::DoGameSpecificStuffAfterSucessLoad() {
    TheText.Load(false);
    CCollision::SortOutCollisionAfterLoad();
    CStreaming::LoadSceneCollision(TheCamera.GetPosition());
    CStreaming::LoadScene(TheCamera.GetPosition());
    CStreaming::LoadAllRequestedModels(false);
    CGame::TidyUpMemory(true, false);
    JustLoadedDontFadeInYet = true;
    StillToFadeOut = true;
    TheCamera.Fade(0.f, eFadeFlag::FADE_IN);
    CTheScripts::Process();
    CTagManager::UpdateNumTagged();
    CClothes::RebuildPlayer(FindPlayerPed(0), false);
    CPopCycle::Update();
    CStreaming::RemoveInappropriatePedModels();
    CGangWars::ClearSpecificZonesToTriggerGangWar();
    CGangWars::bTrainingMission = false;
    CTheZones::FillZonesWithGangColours(CGangWars::bGangWarsActive);
}

// 0x618E70
void CGenericGameStorage::InitRadioStationPositionList() {
    // NOP
}

// 0x618D00
const char* CGenericGameStorage::GetSavedGameDateAndTime(int32 slot) {
    return ms_SlotSaveDate[slot];
}

// 0x5D17B0
bool CGenericGameStorage::GenericLoad(bool& out_bVariablesLoaded) {
    out_bVariablesLoaded = false;

    ms_bFailed = false;
    ms_CheckSum = 0;
    CCheat::ResetCheats();
    if (!OpenFileForReading(nullptr, 0)) {
        return false;
    }
    ms_bLoading = true;

    CSimpleVariablesSaveStructure varsBackup{};

    for (auto block = 0u; block < (uint32)eBlocks::TOTAL; block++) {
        char header[std::size(ms_BlockTagName)]{};
        if (!LoadDataFromWorkBuffer(header, sizeof(header) - 1)) {
            CloseFile();
            return false;
        }

        if (std::string_view{header} != ms_BlockTagName) {
            if (block != 0) {
                ReportError((eBlocks)(block - 1), eSaveLoadError::LOADING);
                if (block == 1) {
                    uint32 version{};
                    varsBackup.Extract(version); // Restore state
                }
            }
            CloseFile();
            ms_bLoading = false;
            return false;
        }

        switch ((eBlocks)block) {
        case eBlocks::SIMPLE_VARIABLES: {
            varsBackup.Construct();

            CSimpleVariablesSaveStructure vars{};
            if (!LoadDataFromWorkBuffer(&vars, sizeof(vars))) {
                ms_bFailed = true;
                break;
            }

            uint32 varsVer{};
            vars.Extract(varsVer);
            if (GetCurrentVersionNumber() != varsVer) {
                fprintf(stderr, "[error] GenericGameStorage: Loading failed (wrong version number = 0x%08x)!", varsVer); // NOTSA
                varsBackup.Extract(varsVer); // Restore old satate
                CloseFile();
                return false;
            }
            break;
        }
        case eBlocks::SCRIPTS:
            CTheScripts::Load();
            break;
        case eBlocks::POOLS:
            if (CPools::Load())
                CTheScripts::DoScriptSetupAfterPoolsHaveLoaded();
            break;
        case eBlocks::GARAGES:
            CGarages::Load();
            break;
        case eBlocks::GAMELOGIC:
            CGameLogic::Load();
            break;
        case eBlocks::PATHS:
            ThePaths.Load();
            break;
        case eBlocks::PICKUPS:
            CPickups::Load();
            break;
        case eBlocks::PHONEINFO: // Unused
             break;
        case eBlocks::RESTART:
            CRestart::Load();
            break;
        case eBlocks::RADAR:
            CRadar::Load();
            break;
        case eBlocks::ZONES:
            CTheZones::Load();
            break;
        case eBlocks::GANGS:
            CGangs::Load();
            break;
        case eBlocks::CAR_GENERATORS:
            CTheCarGenerators::Load();
            break;
        case eBlocks::PED_GENERATORS: // Unused
            break;
        case eBlocks::AUDIO_SCRIPT_OBJECT: // Unused
            break;
        case eBlocks::PLAYERINFO:
            FindPlayerInfo().Load();
            break;
        case eBlocks::STATS:
            CStats::Load();
            break;
        case eBlocks::SET_PIECES:
            CSetPieces::Load();
            break;
        case eBlocks::STREAMING:
            CStreaming::Load();
            break;
        case eBlocks::PED_TYPES:
            CPedType::Load();
            break;
        case eBlocks::TAGS:
            CTagManager::Load();
            break;
        case eBlocks::IPLS:
            CIplStore::Load();
            break;
        case eBlocks::SHOPPING:
            CShopping::Load();
            break;
        case eBlocks::GANGWARS:
            CGangWars::Load();
            break;
        case eBlocks::STUNTJUMPS:
            CStuntJumpManager::Load();
            break;
        case eBlocks::ENTRY_EXITS:
            CEntryExitManager::Load();
            break;
        case eBlocks::RADIOTRACKS:
            CAERadioTrackManager::Load();
            break;
        case eBlocks::USER3DMARKERS:
            C3dMarkers::LoadUser3dMarkers();
            break;
        default:
            assert(0 && "Invalid block"); // NOTSA
            break;
        }

        if (ms_bFailed) {
            ReportError(eBlocks(block), eSaveLoadError::LOADING);
            CloseFile();
            ms_bLoading = false;
            return false;
        }
    }

    ms_bLoading = false;
    if (!CloseFile()) {
        return false;
    }

    DoGameSpecificStuffAfterSucessLoad();

    return true;
}

// 0x5D13E0
bool CGenericGameStorage::GenericSave() {
    ms_bFailed = false;
    if (!OpenFileForWriting()) {
        return false;
    }

    ms_CheckSum = {};

    for (auto block = 0u; block < (uint32)eBlocks::TOTAL; block++) {
        if (!SaveDataToWorkBuffer((void*)ms_BlockTagName, strlen(ms_BlockTagName))) {
            CloseFile();
            return false;
        }

        switch ((eBlocks)block) {
        case eBlocks::SIMPLE_VARIABLES: {
            CSimpleVariablesSaveStructure vars{};
            vars.Construct();
            ms_bFailed = !SaveDataToWorkBuffer((void*)&vars, sizeof(vars));
            break;
        }
        case eBlocks::SCRIPTS:
            CTheScripts::Save();
            break;
        case eBlocks::POOLS:
            CPools::Save();
            break;
        case eBlocks::GARAGES:
            CGarages::Save();
            break;
        case eBlocks::GAMELOGIC:
            CGameLogic::Save();
            break;
        case eBlocks::PATHS:
            ThePaths.Save();
            break;
        case eBlocks::PICKUPS:
            CPickups::Save();
            break;
        case eBlocks::PHONEINFO: // Unused
            break;
        case eBlocks::RESTART:
            CRestart::Save();
            break;
        case eBlocks::RADAR:
            CRadar::Save();
            break;
        case eBlocks::ZONES:
            CTheZones::Save();
            break;
        case eBlocks::GANGS:
            CGangs::Save();
            break;
        case eBlocks::CAR_GENERATORS:
            CTheCarGenerators::Save();
            break;
        case eBlocks::PED_GENERATORS: // Unused
            break;
        case eBlocks::AUDIO_SCRIPT_OBJECT: // Unused
            break;
        case eBlocks::PLAYERINFO:
            FindPlayerInfo().Save();
            break;
        case eBlocks::STATS:
            CStats::Save();
            break;
        case eBlocks::SET_PIECES:
            CSetPieces::Save();
            break;
        case eBlocks::STREAMING:
            CStreaming::Save();
            break;
        case eBlocks::PED_TYPES:
            CPedType::Save();
            break;
        case eBlocks::TAGS:
            CTagManager::Save();
            break;
        case eBlocks::IPLS:
            CIplStore::Save();
            break;
        case eBlocks::SHOPPING:
            CShopping::Save();
            break;
        case eBlocks::GANGWARS:
            CGangWars::Save();
            break;
        case eBlocks::STUNTJUMPS:
            CStuntJumpManager::Save();
            break;
        case eBlocks::ENTRY_EXITS:
            CEntryExitManager::Save();
            break;
        case eBlocks::RADIOTRACKS:
            CAERadioTrackManager::Save();
            break;
        case eBlocks::USER3DMARKERS:
            C3dMarkers::SaveUser3dMarkers();
            break;
        default:
            assert(0 && "Invalid block"); // NOTSA
            break;
        }
    }

    // TODO: Wat is dis?
    while (ms_WorkBufferPos + ms_FilePos < 202748 && (202748 - ms_FilePos) >= 50 * 1024) {
        ms_WorkBufferPos = 50 * 1024;
        if (!SaveWorkBuffer(false)) {
            CloseFile();
            return false;
        }
    }

    if (SaveWorkBuffer(true)) {
        strcpy_s(ms_SaveFileNameJustSaved, ms_SaveFileName);
        if (CloseFile()) {
            CPad::UpdatePads();
            return true;
        }
        return false;
    }
    CloseFile();
    return true;
}

// 0x5D1380
bool CGenericGameStorage::CheckSlotDataValid(int32 slot) {
    return plugin::CallAndReturn<bool, 0x5D1380>(slot);
}

// 0x5D1300
bool CGenericGameStorage::LoadDataFromWorkBuffer(void* data, int32 size) {
    return plugin::CallAndReturn<bool, 0x5D1300, void*, int32>(data, size);
}

// 0x618F50
void CGenericGameStorage::DoGameSpecificStuffBeforeSave() {
    plugin::Call<0x618F50>();
}

// 0x5D1270
int32 CGenericGameStorage::SaveDataToWorkBuffer(void* data, int32 Size) {
    return plugin::CallAndReturn<int32, 0x5D1270, void*, int32>(data, Size);
}

// 0x5D10B0
bool CGenericGameStorage::LoadWorkBuffer() {
    return plugin::CallAndReturn<bool, 0x5D10B0>();
}

const char* CGenericGameStorage::GetBlockName(eBlocks block) {
    switch (block) {
    case eBlocks::SIMPLE_VARIABLES:
        return "SIMPLE_VARIABLES";
    case eBlocks::SCRIPTS:
        return "SCRIPTS";
    case eBlocks::POOLS:
        return "POOLS";
    case eBlocks::GARAGES:
        return "GARAGES";
    case eBlocks::GAMELOGIC:
        return "GAMELOGIC";
    case eBlocks::PATHS:
        return "PATHS";
    case eBlocks::PICKUPS:
        return "PICKUPS";
    case eBlocks::PHONEINFO:
        return "PHONEINFO";
    case eBlocks::RESTART:
        return "RESTART";
    case eBlocks::RADAR:
        return "RADAR";
    case eBlocks::ZONES:
        return "ZONES";
    case eBlocks::GANGS:
        return "GANGS";
    case eBlocks::CAR_GENERATORS:
        return "CAR GENERATORS";
    case eBlocks::PED_GENERATORS:
        return "PED GENERATORS";
    case eBlocks::AUDIO_SCRIPT_OBJECT:
        return "AUDIO SCRIPT OBJECT";
    case eBlocks::PLAYERINFO:
        return "PLAYERINFO";
    case eBlocks::STATS:
        return "STATS";
    case eBlocks::SET_PIECES:
        return "SET PIECES";
    case eBlocks::STREAMING:
        return "STREAMING";
    case eBlocks::PED_TYPES:
        return "PED TYPES";
    case eBlocks::TAGS:
        return "TAGS";
    case eBlocks::IPLS:
        return "IPLS";
    case eBlocks::SHOPPING:
        return "SHOPPING";
    case eBlocks::GANGWARS:
        return "GANGWARS";
    case eBlocks::STUNTJUMPS:
        return "STUNTJUMPS";
    case eBlocks::ENTRY_EXITS:
        return "ENTRY EXITS";
    case eBlocks::RADIOTRACKS:
        return "RADIOTRACKS";
    case eBlocks::USER3DMARKERS:
        return "USER3DMARKERS";
    default:
        return "UNKNOWN";
    }
}

// 0x5D0F80
bool CGenericGameStorage::SaveWorkBuffer(bool bIncludeChecksum) {
    return plugin::CallAndReturn<bool, 0x5D0F80, bool>(bIncludeChecksum);
}

// 0x5D0F50
uint32 CGenericGameStorage::GetCurrentVersionNumber() {
    return plugin::CallAndReturn<uint32, 0x5D0F50>();
}

// 0x5D0E90
char* CGenericGameStorage::MakeValidSaveName(int32 saveNum) {
    return plugin::CallAndReturn<char*, 0x5D0E90, int32>(saveNum);
}

// 0x5D0E30
bool CGenericGameStorage::CloseFile() {
    return plugin::CallAndReturn<bool, 0x5D0E30>();
}

// 0x5D0DD0
bool CGenericGameStorage::OpenFileForWriting() {
    return plugin::CallAndReturn<bool, 0x5D0DD0>();
}

// 0x5D0D20
bool CGenericGameStorage::OpenFileForReading(const char* fileName, int32 slot) {
    return plugin::CallAndReturn<bool, 0x5D0D20>(fileName, slot);
}

// 0x5D1170
bool CGenericGameStorage::CheckDataNotCorrupt(int32 slot, const char* fileName) {
    return plugin::CallAndReturn<bool, 0x5D1170>(slot, fileName);
}

// 0x619000
bool CGenericGameStorage::RestoreForStartLoad() {
    return plugin::CallAndReturn<bool, 0x619000>();
}
