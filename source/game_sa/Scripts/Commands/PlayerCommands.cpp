#include <StdInc.h>

#include "Commands.hpp"
#include <CommandParser/Parser.hpp>

#include <TaskTypes/TaskSimplePlayerOnFoot.h>
#include <TaskTypes/TaskSimpleSwim.h>
#include "PedClothesDesc.h"
#include "MBlur.h"
#include "Ropes.h"

#include <RunningScript.h>

using namespace notsa::script;

/*!
* Various player commands
*/
namespace {
/*!
* @brief Create a player at the given world position
* @param playerId Player's id (0 or 1)
* @param pos      World position
*/
int32 CreatePlayer(int32 playerId, CVector pos) {
    if (!CStreaming::IsModelLoaded(MODEL_PLAYER)) {
        CStreaming::RequestSpecialModel(0, "player", STREAMING_GAME_REQUIRED | STREAMING_KEEP_IN_MEMORY | STREAMING_PRIORITY_REQUEST);
        CStreaming::LoadAllRequestedModels(true);
    }

    // Create
    CPlayerPed::SetupPlayerPed(playerId);
    auto player = FindPlayerPed(playerId);
    player->SetCharCreatedBy(PED_MISSION);
    CPlayerPed::DeactivatePlayerPed(playerId);

    // Position in the world
    if (pos.z <= MAP_Z_LOW_LIMIT) {
        pos.z = CWorld::FindGroundZForCoord(pos.x, pos.y);
    }
    pos.z += player->GetDistanceFromCentreOfMassToBaseOfModel();
    player->SetPosn(pos);
    CTheScripts::ClearSpaceForMissionEntity(pos, player);
    CPlayerPed::ReactivatePlayerPed(playerId);

    // Set task
    player->GetTaskManager().SetTask(new CTaskSimplePlayerOnFoot(), TASK_PRIMARY_DEFAULT);

    return playerId;
}

/// Get the position of a player
CVector GetPlayerCoordinates(CPlayerInfo& pinfo) {
    return pinfo.GetPos();
}

bool IsPlayerInArea2D(CRunningScript* S, CPlayerPed& player, CRect area, bool highlightArea) {
    if (highlightArea) {
        CTheScripts::HighlightImportantArea((uint32)S + (uint32)S->m_IP, area, MAP_Z_LOW_LIMIT);
    }

    if (CTheScripts::DbgFlag) {
        CTheScripts::DrawDebugSquare(area);
    }

    return player.bInVehicle
        ? player.m_pVehicle->IsWithinArea(area.left, area.top, area.right, area.bottom)
        : player.IsWithinArea(area.left, area.top, area.right, area.bottom);
}

bool IsPlayerInArea3D(CRunningScript* S, CPlayerPed& player, CVector p1, CVector p2, bool highlightArea) {
    if (highlightArea) {
        CTheScripts::HighlightImportantArea((uint32)S + (uint32)S->m_IP, p1.x, p1.y, p2.x, p2.y, (p1.z + p2.z) / 2.0f);
    }

    if (CTheScripts::DbgFlag) {
        CTheScripts::DrawDebugCube(p1, p2);
    }

    return player.bInVehicle
        ? player.m_pVehicle->IsWithinArea(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z)
        : player.IsWithinArea(p1.x, p1.y, p1.z, p2.x, p2.y, p2.z);
}

auto IsPlayerPlaying(CPlayerInfo& player) -> notsa::script::CompareFlagUpdate {
    return { player.m_nPlayerState == PLAYERSTATE_PLAYING };
}

bool IsPlayerClimbing(CPlayerPed& player) {
    return player.GetIntelligence()->GetTaskClimb();
}

void SetSwimSpeed(CPlayerPed& player, float speed) {
    if (auto swim = player.GetIntelligence()->GetTaskSwim()) {
        swim->m_fAnimSpeed = speed;
    }
}

void SetPlayerGroupToFollowAlways(CPlayerPed& player, bool enable) {
    player.ForceGroupToAlwaysFollow(enable);
}

void SetPlayerAbleToUseCrouch(uint32 playerIdx, bool enable) {
    CPad::GetPad(playerIdx)->bDisablePlayerDuck = !enable;
}

// Originally unsupported
bool IsPlayerTouchingObject(CPlayerPed& player, CObject& object) {
    return GetPedOrItsVehicle(player).GetHasCollidedWith(&object);
}

// Originally unsupported
bool IsPlayerTouchingObjectOnFoot(CPlayerPed& player, CObject& object) {
    return player.GetHasCollidedWith(&object);
}

// COMMAND_IS_PLAYER_IN_POSITION_FOR_CONVERSATION - 0x474983
//bool IsPlayerInPositionForConversation(CPed& ped) { // Yes, it's CPed
//
//}

void AddScore(CPlayerInfo& player, int32 score) {
    player.m_nMoney += score;
}

//
bool IsScoreGreater(CPlayerInfo& player, int32 score) {
    return player.m_nMoney > score;
}

int32 StoreScore(CPlayerInfo& player) {
    return player.m_nMoney;
}

void AlterPlayerWantedLevel(CPlayerPed& player, int32 level) {
    player.SetWantedLevel(level);
}

void AlterPlayerWantedLevelNoDrop(CPlayerPed& player, int32 level) {
    player.SetWantedLevelNoDrop(level);
}

bool IsWantedLevelGreater(CPlayerPed& player, int32 level) {
    return (int32)player.GetWanted()->GetWantedLevel() > level;
}

void ClearWantedLevel(CPlayerPed& player) {
    player.GetWanted()->SetWantedLevel(0);
}

void SetMaxWantedLevel(int32 level) {
    CWanted::SetMaximumWantedLevel(level);
}

bool IsPlayerDead(CPlayerInfo& player) {
    return player.m_nPlayerState == PLAYERSTATE_HAS_DIED;
}

int32 GetPlayerChar(CPlayerPed& player) {
    return GetPedPool()->GetRef(&player);
}

bool IsPlayerPressingHorn(uint32 playerIdx) {
    return FindPlayerPed(playerIdx)->GetPedState() == PEDSTATE_DRIVING && CPad::GetPad(playerIdx)->GetHorn();
}

void SetPlayerControl(CPlayerInfo& player, bool state) {
    player.MakePlayerSafe(!state, 10.0);
}

void SetPoliceIgnorePlayer(CPlayerPed& player, bool state) {
    player.GetWanted()->m_bPoliceBackOff = state;
    if (state) {
        CWorld::StopAllLawEnforcersInTheirTracks();
    }
}

void SetEveryoneIgnorePlayer(CPlayerPed& player, bool state) {
    player.GetWanted()->m_bEverybodyBackOff = state;
    if (state) {
        CWorld::StopAllLawEnforcersInTheirTracks();
    }
}

void ApplyBrakesToPlayersCar(uint32 playerIdx, bool state) {
    CPad::GetPad(playerIdx)->bApplyBrakes = state;
}

bool IsPlayerInRemoteMode(CPlayerInfo& player) {
    return player.IsPlayerInRemoteMode();
}

int16 GetNumOfModelsKilledByPlayer(uint32 playerIdx, eModelID model) {
    return CDarkel::QueryModelsKilledByPlayer(playerIdx, model);
}

void ResetNumOfModelsKilledByPlayer(uint32 playerIdx) {
    CDarkel::ResetModelsKilledByPlayer(playerIdx);
}

void SetPlayerNeverGetsTired(CPlayerInfo& player, bool state) {
    player.m_bDoesNotGetTired = state;
}

void SetPlayerFastReload(CPlayerInfo& player, bool state) {
    player.m_bFastReload = state;
}

bool CanPlayerStartMission(CPlayerPed& player) {
    return player.CanPlayerStartMission();
}

void MakePlayerSafeForCutscene(uint32 playerIdx) {
    CPad::GetPad(playerIdx)->bPlayerSafeForCutscene = true;
    FindPlayerInfo(playerIdx).MakePlayerSafe(true, 10000.0f);
    CCutsceneMgr::ms_cutsceneProcessing = 1;
}

bool IsPlayerTargettingChar(CPlayerPed& player, CPed* target) {
    CEntity* targetedObject = player.m_pTargetedObject;
    if (targetedObject && targetedObject->GetType() == ENTITY_TYPE_PED && targetedObject == target) {
        return true;
    }

    if (CCamera::m_bUseMouse3rdPerson && player.GetPadFromPlayer()->GetTarget()) {
        return player.m_p3rdPersonMouseTarget && player.m_p3rdPersonMouseTarget == target;
    }

    return false;
}

bool IsPlayerTargettingObject(CPlayerPed& player, CObject* target) {
    CEntity* targetedObject = player.m_pTargetedObject;
    return targetedObject && targetedObject->GetType() == ENTITY_TYPE_OBJECT && targetedObject == target;
}

void GiveRemoteControlledModelToPlayer(CPlayerPed& player, CVector posn, float angle, eModelID model) {
    if (posn.z <= MAP_Z_LOW_LIMIT) {
        posn.z = CWorld::FindGroundZForCoord(posn.x, posn.y);
    }
    CRemote::GivePlayerRemoteControlledCar(posn, DegreesToRadians(angle), model);
}

void MakePlayerFireProof(CPlayerInfo& player, bool state) {
    player.m_bFireProof = state;
}

uint8 GetPlayerMaxArmour(CPlayerInfo& player) {
    return player.m_nMaxArmour;
}

void SetPlayerMood(CPlayerPed& player, eCJMood mood, uint32 time) {
    CAEPedSpeechAudioEntity::SetCJMood(mood, time);
}

bool IsPlayerWearing(CPlayerPed& player, eClothesTexturePart bodyPart, std::string_view textureName) {
    return player.GetClothesDesc()->m_anTextureKeys[bodyPart] == CKeyGen::GetUppercaseKey(textureName.data());
}

void SetPlayerCanDoDriveBy(CPlayerInfo& player, bool state) {
    player.m_bCanDoDriveBy = state;
}

void SetPlayerDrunkenness(CPlayerInfo& player, uint8 intensity) {
    player.m_PlayerData.m_nDrunkenness = intensity;
    player.m_PlayerData.m_nFadeDrunkenness = 0;
    if (!intensity) {
        CMBlur::ClearDrunkBlur();
    }
}

void IncreasePlayerMaxHealth(CPlayerInfo& player, uint8 value) {
    player.m_nMaxHealth += value;
    player.m_pPed->m_fHealth += value;
}

void IncreasePlayerMaxArmour(CPlayerInfo& player, uint8 value) {
    player.m_nMaxArmour += value;
    player.m_pPed->m_fArmour += value;
}

void EnsurePlayerHasDriveByWeapon(CPlayerPed& player, uint32 ammo) {
    if (!player.bInVehicle) {
        return;
    }
    auto type = player.GetWeaponInSlot(eWeaponSlot::SMG).GetType();
    if (type) {
        if (player.GetWeaponInSlot(eWeaponSlot::SMG).GetTotalAmmo() < ammo) {
            player.SetAmmo(type, ammo);
        }
        return;
    }

    player.GiveWeapon(eWeaponType::WEAPON_MICRO_UZI, ammo, true);
    if (player.GetSavedWeapon() == eWeaponType::WEAPON_UNIDENTIFIED) {
        player.SetSavedWeapon(player.GetActiveWeapon().GetType());
    }
    player.SetCurrentWeapon(eWeaponType::WEAPON_MICRO_UZI);
}

bool IsPlayerInInfoZone(CPlayerInfo& player, std::string_view zoneName) {
    return CTheZones::FindZone(player.GetPos(), zoneName, eZoneType::ZONE_TYPE_INFO);
}

bool IsPlayerTargettingAnything(CPlayerPed& player) {
    return player.m_pTargetedObject;
}

void DisablePlayerSprint(CPlayerInfo& player, bool state) {
    player.m_PlayerData.m_bPlayerSprintDisabled = state;
}

void DeletePlayer(uint32 playerIdx) {
    CPlayerPed::RemovePlayerPed(playerIdx);
}

void BuildPlayerModel(CPlayerPed* player) {
    CClothes::RebuildPlayer(player, false);
    CReplay::Init();
}

void GivePlayerClothes(CPlayerPed& player, uint32 textureHash, uint32 modelHash, eClothesTexturePart bodyPart) {
    player.GetClothesDesc()->SetTextureAndModel(textureHash, modelHash, bodyPart);
}

void PlayerEnteredBuildingsiteCrane() {
    CRopes::PlayerControlsCrane = eControlledCrane::WRECKING_BALL;
    CWaterLevel::m_bWaterFogScript = false;
}

void PlayerEnteredDockCrane() {
    CRopes::PlayerControlsCrane = eControlledCrane::MAGNO_CRANE;
    CWaterLevel::m_bWaterFogScript = false;
}

void PlayerEnteredLasVegasCrane() {
    CRopes::PlayerControlsCrane = eControlledCrane::LAS_VEGAS_CRANE;
    CWaterLevel::m_bWaterFogScript = false;
}

void PlayerEnteredQuarryCrane() {
    CRopes::PlayerControlsCrane = eControlledCrane::QUARRY_CRANE;
}

void PlayerLeftCrane() {
    CRopes::PlayerControlsCrane = eControlledCrane::NONE;
    CWaterLevel::m_bWaterFogScript = true;
}

int32 GetPlayerGroup(CPlayerInfo& player) {
    return CTheScripts::GetUniqueScriptThingIndex(player.m_PlayerData.m_nPlayerGroup, eScriptThingType::SCRIPT_THING_PED_GROUP);
}

void SetPlayerGroupRecruitment(CPlayerInfo& player, bool state) {
    player.m_PlayerData.m_bGroupStuffDisabled = !state;
}

void SetPlayerEnterCarButton(uint32 playerIdx, bool state) {
    CPad::GetPad(playerIdx)->bDisablePlayerEnterCar = !state;
}

bool IsPlayerPerformingWheelie(CPlayerInfo& player) {
    return player.m_pPed->bInVehicle && player.m_pPed->m_pVehicle->GetVehicleAppearance() == eVehicleAppearance::VEHICLE_APPEARANCE_BIKE && player.m_nBikeRearWheelCounter;
}

bool IsPlayerPerformingStoppie(CPlayerInfo& player) {
    return player.m_pPed->bInVehicle && player.m_pPed->m_pVehicle->GetVehicleAppearance() == eVehicleAppearance::VEHICLE_APPEARANCE_BIKE && player.m_nBikeFrontWheelCounter;
}

};

void notsa::script::commands::player::RegisterHandlers() {
    REGISTER_COMMAND_HANDLER_BEGIN("Player");

    /* 0053 */ REGISTER_COMMAND_HANDLER(COMMAND_CREATE_PLAYER, CreatePlayer);
    /* 0109 */ REGISTER_COMMAND_HANDLER(COMMAND_ADD_SCORE, AddScore);
    /* 010A */ REGISTER_COMMAND_HANDLER(COMMAND_IS_SCORE_GREATER, IsScoreGreater);
    /* 010B */ REGISTER_COMMAND_HANDLER(COMMAND_STORE_SCORE, StoreScore);
    /* 010D */ REGISTER_COMMAND_HANDLER(COMMAND_ALTER_WANTED_LEVEL, AlterPlayerWantedLevel);
    /* 010E */ REGISTER_COMMAND_HANDLER(COMMAND_ALTER_WANTED_LEVEL_NO_DROP, AlterPlayerWantedLevelNoDrop);
    /* 010F */ REGISTER_COMMAND_HANDLER(COMMAND_IS_WANTED_LEVEL_GREATER, IsWantedLevelGreater);
    /* 0110 */ REGISTER_COMMAND_HANDLER(COMMAND_CLEAR_WANTED_LEVEL, ClearWantedLevel);
    /* 0117 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_DEAD, IsPlayerDead);
    /* 0122 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_PRESSING_HORN, IsPlayerPressingHorn);
    /* 01B4 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_CONTROL, SetPlayerControl);
    /* 01F0 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_MAX_WANTED_LEVEL, SetMaxWantedLevel);
    /* 01F5 */ REGISTER_COMMAND_HANDLER(COMMAND_GET_PLAYER_CHAR, GetPlayerChar);
    /* 01F7 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_POLICE_IGNORE_PLAYER, SetPoliceIgnorePlayer);
    /* 0221 */ REGISTER_COMMAND_HANDLER(COMMAND_APPLY_BRAKES_TO_PLAYERS_CAR, ApplyBrakesToPlayersCar);
    /* 0241 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_IN_REMOTE_MODE, IsPlayerInRemoteMode);
    /* 0256 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_PLAYING, IsPlayerPlaying);
    /* 0297 */ REGISTER_COMMAND_HANDLER(COMMAND_RESET_NUM_OF_MODELS_KILLED_BY_PLAYER, ResetNumOfModelsKilledByPlayer);
    /* 0298 */ REGISTER_COMMAND_HANDLER(COMMAND_GET_NUM_OF_MODELS_KILLED_BY_PLAYER, GetNumOfModelsKilledByPlayer);
    /* 0330 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_NEVER_GETS_TIRED, SetPlayerNeverGetsTired);
    /* 0331 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_FAST_RELOAD, SetPlayerFastReload);
    /* 03BF */ REGISTER_COMMAND_HANDLER(COMMAND_SET_EVERYONE_IGNORE_PLAYER, SetEveryoneIgnorePlayer);
    /* 03EE */ REGISTER_COMMAND_HANDLER(COMMAND_CAN_PLAYER_START_MISSION, CanPlayerStartMission);
    /* 03EF */ REGISTER_COMMAND_HANDLER(COMMAND_MAKE_PLAYER_SAFE_FOR_CUTSCENE, MakePlayerSafeForCutscene);
    /* 0457 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_TARGETTING_CHAR, IsPlayerTargettingChar);
    /* 0458 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_TARGETTING_OBJECT, IsPlayerTargettingObject);
    /* 046E */ REGISTER_COMMAND_HANDLER(COMMAND_GIVE_REMOTE_CONTROLLED_MODEL_TO_PLAYER, GiveRemoteControlledModelToPlayer);
    /* 04E3 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_MOOD, SetPlayerMood);
    /* 0500 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_WEARING, IsPlayerWearing);
    /* 0501 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_CAN_DO_DRIVE_BY, SetPlayerCanDoDriveBy);
    /* 052C */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_DRUNKENNESS, SetPlayerDrunkenness);
    /* 055D */ REGISTER_COMMAND_HANDLER(COMMAND_MAKE_PLAYER_FIRE_PROOF, MakePlayerFireProof);
    /* 055E */ REGISTER_COMMAND_HANDLER(COMMAND_INCREASE_PLAYER_MAX_HEALTH, IncreasePlayerMaxHealth);
    /* 055F */ REGISTER_COMMAND_HANDLER(COMMAND_INCREASE_PLAYER_MAX_ARMOUR, IncreasePlayerMaxArmour);
    /* 0563 */ REGISTER_COMMAND_HANDLER(COMMAND_ENSURE_PLAYER_HAS_DRIVE_BY_WEAPON, EnsurePlayerHasDriveByWeapon);
    /* 0583 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_IN_INFO_ZONE, IsPlayerInInfoZone);
    /* 068C */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_TARGETTING_ANYTHING, IsPlayerTargettingAnything);
    /* 06AF */ REGISTER_COMMAND_HANDLER(COMMAND_DISABLE_PLAYER_SPRINT, DisablePlayerSprint);
    /* 06DF */ REGISTER_COMMAND_HANDLER(COMMAND_DELETE_PLAYER, DeletePlayer);
    /* 070D */ REGISTER_COMMAND_HANDLER(COMMAND_BUILD_PLAYER_MODEL, BuildPlayerModel);
    /* 0784 */ REGISTER_COMMAND_HANDLER(COMMAND_GIVE_PLAYER_CLOTHES, GivePlayerClothes);
    /* 079D */ REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_ENTERED_DOCK_CRANE, PlayerEnteredDockCrane);
    /* 079E */ REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_ENTERED_BUILDINGSITE_CRANE, PlayerEnteredBuildingsiteCrane);
    /* 079F */ REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_LEFT_CRANE, PlayerLeftCrane);
    /* 07AF */ REGISTER_COMMAND_HANDLER(COMMAND_GET_PLAYER_GROUP, GetPlayerGroup);
    /* 07B4 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_GROUP_RECRUITMENT, SetPlayerGroupRecruitment);
    /* 07CC */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_ENTER_CAR_BUTTON, SetPlayerEnterCarButton);
    /* 07F1 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_PERFORMING_WHEELIE, IsPlayerPerformingWheelie);
    /* 07F2 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_PERFORMING_STOPPIE, IsPlayerPerformingStoppie);
    /* 07F9 */ REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_ENTERED_QUARRY_CRANE, PlayerEnteredQuarryCrane);
    /* 07FA */ REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_ENTERED_LAS_VEGAS_CRANE, PlayerEnteredLasVegasCrane);
    /* 082A */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_DUCK_BUTTON, SetPlayerAbleToUseCrouch);
    /* 0945 */ REGISTER_COMMAND_HANDLER(COMMAND_GET_PLAYER_MAX_ARMOUR, GetPlayerMaxArmour);
    /* 0A20 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_GROUP_TO_FOLLOW_ALWAYS, SetPlayerGroupToFollowAlways);
    /* 0A28 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_SWIM_SPEED, SetSwimSpeed);
    /* 0A29 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_CLIMBING, IsPlayerClimbing);

    // TODO:
    /* 03C8 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_CAMERA_IN_FRONT_OF_PLAYER, SetCameraInFrontOfPlayer);
    /* 043E */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_HOOKER, SetPlayerHooker);
    /* 0450 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_JAMES_CAR_ON_PATH_TO_PLAYER, SetJamesCarOnPathToPlayer);
    /* 04BE */ //REGISTER_COMMAND_HANDLER(COMMAND_RESET_HAVOC_CAUSED_BY_PLAYER, ResetHavocCausedByPlayer);
    /* 04BF */ //REGISTER_COMMAND_HANDLER(COMMAND_GET_HAVOC_CAUSED_BY_PLAYER, GetHavocCausedByPlayer);
    /* 0501 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_CAN_DO_DRIVE_BY, SetPlayerCanDoDriveBy);
    /* 0540 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_AUTO_AIM, SetPlayerAutoAim);
    /* 0548 */ //REGISTER_COMMAND_HANDLER(COMMAND_CHECK_FOR_PED_MODEL_AROUND_PLAYER, CheckForPedModelAroundPlayer);
    /* 0551 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_HAS_MET_DEBBIE_HARRY, SetPlayerHasMetDebbieHarry);
    /* 057F */ //REGISTER_COMMAND_HANDLER(COMMAND_GET_BUS_FARES_COLLECTED_BY_PLAYER, GetBusFaresCollectedByPlayer);
    /* 0592 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_GANG_ATTACK_PLAYER_WITH_COPS, SetGangAttackPlayerWithCops);
    /* 05B7 */ //REGISTER_COMMAND_HANDLER(COMMAND_TASK_PLAYER_ON_FOOT, TaskPlayerOnFoot);
    /* 05B8 */ //REGISTER_COMMAND_HANDLER(COMMAND_TASK_PLAYER_IN_CAR, TaskPlayerInCar);
    /* 068F */ //REGISTER_COMMAND_HANDLER(COMMAND_GET_CLOSEST_BUYABLE_OBJECT_TO_PLAYER, GetClosestBuyableObjectToPlayer);
    /* 06E0 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAMERA_MODE, SetTwoPlayerCameraMode);
    /* 06F1 */ //REGISTER_COMMAND_HANDLER(COMMAND_LIMIT_TWO_PLAYER_DISTANCE, LimitTwoPlayerDistance);
    /* 06F2 */ //REGISTER_COMMAND_HANDLER(COMMAND_RELEASE_TWO_PLAYER_DISTANCE, ReleaseTwoPlayerDistance);
    /* 06F3 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_PLAYER_TARGETTING, SetPlayerPlayerTargetting);
    /* 06F6 */ //REGISTER_COMMAND_HANDLER(COMMAND_CLEAR_TWO_PLAYER_CAMERA_MODE, ClearTwoPlayerCameraMode);
    /* 06F7 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_PASSENGER_CAN_SHOOT, SetPlayerPassengerCanShoot);
    /* 06FA */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYERS_CAN_BE_IN_SEPARATE_CARS, SetPlayersCanBeInSeparateCars);
    /* 06FB */ //REGISTER_COMMAND_HANDLER(COMMAND_SWITCH_PLAYER_CROSSHAIR, SwitchPlayerCrosshair);
    /* 0785 */ //REGISTER_COMMAND_HANDLER(COMMAND_GIVE_PLAYER_TATTOO, GivePlayerTattoo);
    /* 07CF */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_SEPARATE_CARS, SetTwoPlayerCamModeSeparateCars);
    /* 0807 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_SAME_CAR_SHOOTING, SetTwoPlayerCamModeSameCarShooting);
    /* 0808 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_SAME_CAR_NO_SHOOTING, SetTwoPlayerCamModeSameCarNoShooting);
    /* 0809 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_NOT_BOTH_IN_CAR, SetTwoPlayerCamModeNotBothInCar);
    /* 0881 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_FIRE_BUTTON, SetPlayerFireButton);
    /* 089B */ //REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_IN_POSITION_FOR_CONVERSATION, IsPlayerInPositionForConversation);
    /* 08A2 */ //REGISTER_COMMAND_HANDLER(COMMAND_PLANE_ATTACK_PLAYER_USING_DOG_FIGHT, PlaneAttackPlayerUsingDogFight);
    /* 0901 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_JUMP_BUTTON, SetPlayerJumpButton);
    /* 092A */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_CAN_BE_DAMAGED, SetPlayerCanBeDamaged);
    /* 092C */ //REGISTER_COMMAND_HANDLER(COMMAND_GET_PLAYERS_GANG_IN_CAR_ACTIVE, GetPlayersGangInCarActive);
    /* 092D */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYERS_GANG_IN_CAR_ACTIVE, SetPlayersGangInCarActive);
    /* 0942 */ //REGISTER_COMMAND_HANDLER(COMMAND_HAS_PLAYER_BOUGHT_ITEM, HasPlayerBoughtItem);
    /* 0999 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_FIRE_WITH_SHOULDER_BUTTON, SetPlayerFireWithShoulderButton);
    /* 09EA */ //REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_PUT_ON_GOGGLES, PlayerPutOnGoggles);
    /* 0A38 */ //REGISTER_COMMAND_HANDLER(COMMAND_SET_RENDER_PLAYER_WEAPON, SetRenderPlayerWeapon);

    // -----------------------------[ NOP ]-----------------------------
    auto constexpr NOP = []() {};
    /* 010C */ REGISTER_COMMAND_HANDLER(COMMAND_GIVE_REMOTE_CONTROLLED_CAR_TO_PLAYER, NOP);
    /* 04E2 */ REGISTER_COMMAND_HANDLER(COMMAND_SHUT_PLAYER_UP, [](uint32, uint32) {});
    /* 0596 */ REGISTER_COMMAND_HANDLER(COMMAND_IS_PLAYER_IN_SHORTCUT_TAXI, [](uint32) {});
    /* 043E */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_HOOKER, NOP);
    /* 0450 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_JAMES_CAR_ON_PATH_TO_PLAYER, NOP);
    /* 04BE */ REGISTER_COMMAND_HANDLER(COMMAND_RESET_HAVOC_CAUSED_BY_PLAYER, NOP);
    /* 04BF */ REGISTER_COMMAND_HANDLER(COMMAND_GET_HAVOC_CAUSED_BY_PLAYER, NOP);
    /* 0540 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_AUTO_AIM, NOP);
    /* 0548 */ REGISTER_COMMAND_HANDLER(COMMAND_CHECK_FOR_PED_MODEL_AROUND_PLAYER, NOP);
    /* 0551 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_HAS_MET_DEBBIE_HARRY, NOP);
    /* 057F */ REGISTER_COMMAND_HANDLER(COMMAND_GET_BUS_FARES_COLLECTED_BY_PLAYER, NOP);
    /* 0592 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_GANG_ATTACK_PLAYER_WITH_COPS, NOP);
    /* 05B7 */ REGISTER_COMMAND_HANDLER(COMMAND_TASK_PLAYER_ON_FOOT, NOP);
    /* 05B8 */ REGISTER_COMMAND_HANDLER(COMMAND_TASK_PLAYER_IN_CAR, NOP);
    /* 068F */ REGISTER_COMMAND_HANDLER(COMMAND_GET_CLOSEST_BUYABLE_OBJECT_TO_PLAYER, NOP);
    /* 06F6 */ REGISTER_COMMAND_HANDLER(COMMAND_CLEAR_TWO_PLAYER_CAMERA_MODE, NOP);
    /* 06F7 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_PASSENGER_CAN_SHOOT, NOP);
    /* 06FB */ REGISTER_COMMAND_HANDLER(COMMAND_SWITCH_PLAYER_CROSSHAIR, NOP);
    /* 0785 */ REGISTER_COMMAND_HANDLER(COMMAND_GIVE_PLAYER_TATTOO, NOP);
    /* 07CF */ REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_SEPARATE_CARS, NOP);
    /* 0807 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_SAME_CAR_SHOOTING, NOP);
    /* 0808 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_SAME_CAR_NO_SHOOTING, NOP);
    /* 0809 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_TWO_PLAYER_CAM_MODE_NOT_BOTH_IN_CAR, NOP);
    /* 092A */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_CAN_BE_DAMAGED, NOP);
    /* 092C */ REGISTER_COMMAND_HANDLER(COMMAND_GET_PLAYERS_GANG_IN_CAR_ACTIVE, NOP);
    /* 092D */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYERS_GANG_IN_CAR_ACTIVE, NOP);
    /* 0999 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_PLAYER_FIRE_WITH_SHOULDER_BUTTON, NOP);
    /* 09EA */ REGISTER_COMMAND_HANDLER(COMMAND_PLAYER_PUT_ON_GOGGLES, NOP);
    /* 0A38 */ REGISTER_COMMAND_HANDLER(COMMAND_SET_RENDER_PLAYER_WEAPON, NOP);

    // -------------------------[ UNSUPPORTED ]-------------------------
    /* 0054 */ REGISTER_UNSUPPORTED_COMMAND_HANDLER(COMMAND_GET_PLAYER_COORDINATES, GetPlayerCoordinates);
    /* 0055 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_COORDINATES);
    /* 0056 */ REGISTER_UNSUPPORTED_COMMAND_HANDLER(COMMAND_IS_PLAYER_IN_AREA_2D, IsPlayerInArea2D);
    /* 0057 */ REGISTER_UNSUPPORTED_COMMAND_HANDLER(COMMAND_IS_PLAYER_IN_AREA_3D, IsPlayerInArea3D);
    /* 00DA */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_STORE_CAR_PLAYER_IS_IN);
    /* 00DC */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_CAR);
    /* 00DE */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_MODEL);
    /* 00E0 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANY_CAR);
    /* 00E3 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ANY_MEANS_2D);
    /* 00E4 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ON_FOOT_2D);
    /* 00E5 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_IN_CAR_2D);
    /* 00E6 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_STOPPED_PLAYER_ANY_MEANS_2D);
    /* 00E7 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_STOPPED_PLAYER_ON_FOOT_2D);
    /* 00E8 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_STOPPED_PLAYER_IN_CAR_2D);
    /* 00F5 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ANY_MEANS_3D);
    /* 00F6 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ON_FOOT_3D);
    /* 00F7 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_IN_CAR_3D);
    /* 00F8 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_STOPPED_PLAYER_ANY_MEANS_3D);
    /* 00F9 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_STOPPED_PLAYER_ON_FOOT_3D);
    /* 00FA */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_STOPPED_PLAYER_IN_CAR_3D);
    /* 012A */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_WARP_PLAYER_FROM_CAR_TO_COORD);
    /* 0170 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GET_PLAYER_HEADING);
    /* 0171 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_HEADING);
    /* 0178 */ REGISTER_UNSUPPORTED_COMMAND_HANDLER(COMMAND_IS_PLAYER_TOUCHING_OBJECT, IsPlayerTouchingObject);
    /* 0183 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_HEALTH_GREATER);
    /* 0197 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_AREA_ON_FOOT_2D);
    /* 0198 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_AREA_IN_CAR_2D);
    /* 0199 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_AREA_2D);
    /* 019A */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_AREA_ON_FOOT_2D);
    /* 019B */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_AREA_IN_CAR_2D);
    /* 019C */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_AREA_ON_FOOT_3D);
    /* 019D */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_AREA_IN_CAR_3D);
    /* 019E */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_AREA_3D);
    /* 019F */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_AREA_ON_FOOT_3D);
    /* 01A0 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_AREA_IN_CAR_3D);
    /* 01B1 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GIVE_WEAPON_TO_PLAYER);
    /* 01B8 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_CURRENT_PLAYER_WEAPON);
    /* 01BF */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_TURN_PLAYER_TO_FACE_COORD);
    /* 01DF */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_AS_LEADER);
    /* 01FC */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ANY_MEANS_CAR_2D);
    /* 01FD */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ON_FOOT_CAR_2D);
    /* 01FE */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_IN_CAR_CAR_2D);
    /* 01FF */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ANY_MEANS_CAR_3D);
    /* 0200 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_ON_FOOT_CAR_3D);
    /* 0201 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_LOCATE_PLAYER_IN_CAR_CAR_3D);
    /* 0210 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_TURN_PLAYER_TO_FACE_CHAR);
    /* 0222 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_HEALTH);
    /* 0225 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GET_PLAYER_HEALTH);
    /* 0230 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_STOP_PLAYER_LOOKING);
    /* 0234 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_GANG_PLAYER_ATTITUDE);
    /* 023A */ REGISTER_UNSUPPORTED_COMMAND_HANDLER(COMMAND_IS_PLAYER_TOUCHING_OBJECT_ON_FOOT, IsPlayerTouchingObjectOnFoot);
    /* 0246 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_ANIM_GROUP_FOR_PLAYER);
    /* 0292 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_HEED_THREATS);
    /* 029F */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED);
    /* 02AD */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANGLED_AREA_2D);
    /* 02AE */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANGLED_AREA_ON_FOOT_2D);
    /* 02AF */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANGLED_AREA_IN_CAR_2D);
    /* 02B0 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_ANGLED_AREA_2D);
    /* 02B1 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_ANGLED_AREA_ON_FOOT_2D);
    /* 02B2 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_ANGLED_AREA_IN_CAR_2D);
    /* 02B3 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANGLED_AREA_3D);
    /* 02B4 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANGLED_AREA_ON_FOOT_3D);
    /* 02B5 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANGLED_AREA_IN_CAR_3D);
    /* 02B6 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_ANGLED_AREA_3D);
    /* 02B7 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_ANGLED_AREA_ON_FOOT_3D);
    /* 02B8 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STOPPED_IN_ANGLED_AREA_IN_CAR_3D);
    /* 02D5 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_SHOOTING_IN_AREA);
    /* 02D7 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_CURRENT_PLAYER_WEAPON);
    /* 02DE */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_TAXI);
    /* 02DF */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_SHOOTING);
    /* 0322 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_EXPLODE_PLAYER_HEAD);
    /* 0336 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_VISIBLE);
    /* 035E */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_ADD_ARMOUR_TO_PLAYER);
    /* 0367 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_START_KILL_FRENZY_HEADSHOT);
    /* 0368 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_ACTIVATE_MILITARY_CRANE);
    /* 0369 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_WARP_PLAYER_INTO_CAR);
    /* 037F */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GIVE_PLAYER_DETONATOR);
    /* 03C1 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_STORE_CAR_PLAYER_IS_IN_NO_SAVE);
    /* 03F6 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_MAKE_PLAYER_UNSAFE);
    /* 03FA */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_MAKE_PLAYER_SAFE);
    /* 0419 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GET_AMMO_IN_PLAYER_WEAPON);
    /* 0442 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_SITTING_IN_CAR);
    /* 0443 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_SITTING_IN_ANY_CAR);
    /* 0447 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_LIFTING_A_PHONE);
    /* 044A */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_ON_FOOT);
    /* 0452 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_ENABLE_PLAYER_CONTROL_CAMERA);
    /* 0456 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_TARGETTING_ANY_CHAR);
    /* 046F */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GET_CURRENT_PLAYER_WEAPON);
    /* 047E */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_ON_ANY_BIKE);
    /* 0490 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_HAS_PLAYER_GOT_WEAPON);
    /* 0492 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_FACING_CHAR);
    /* 0499 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_STANDING_ON_A_VEHICLE);
    /* 049A */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_FOOT_DOWN);
    /* 04A8 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANY_BOAT);
    /* 04AA */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANY_HELI);
    /* 04AC */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_ANY_PLANE);
    /* 04C9 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_IN_FLYING_VEHICLE);
    /* 052D */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GET_PLAYER_DRUNKENNESS);
    /* 052E */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_SET_PLAYER_DRUG_LEVEL);
    /* 052F */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_GET_PLAYER_DRUG_LEVEL);
    /* 0546 */ REGISTER_COMMAND_UNIMPLEMENTED(COMMAND_IS_PLAYER_TOUCHING_VEHICLE);
}
