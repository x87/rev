#include "StdInc.h"
#include "Conversations.h"
#include "IKChainManager_c.h"

void CConversations::InjectHooks() {
    RH_ScopedClass(CConversations);
    RH_ScopedCategoryGlobal();

    RH_ScopedInstall(Clear, 0x43A7B0);
    RH_ScopedInstall(Update, 0x43C590);
    RH_ScopedInstall(SetUpConversationNode, 0x43A870);
    RH_ScopedInstall(RemoveConversationForPed, 0x43A960);
    RH_ScopedInstall(IsPlayerInPositionForConversation, 0x43B0B0);
    RH_ScopedInstall(IsConversationGoingOn, 0x43AAC0);
    RH_ScopedInstall(IsConversationAtNode, 0x43B000);
    RH_ScopedInstall(AwkwardSay, 0x43A810);
    RH_ScopedInstall(EnableConversation, 0x43A7F0);
    RH_ScopedInstall(StartSettingUpConversation, 0x43A840);
    RH_ScopedInstall(DoneSettingUpConversation, 0x43ADB0, {.reversed = false});

}


// 0x43AAE0
void CPedToPlayerConversations::Clear() {
    ZoneScoped;

    if (m_State != eP2pState::INACTIVE) {
        CAEPedSpeechAudioEntity::ReleasePlayerConversation();
        m_State = eP2pState::INACTIVE;
    }

    m_pPed                         = nullptr;
    m_TimeOfLastPlayerConversation = 0;
}

// 0x43B0F0
void CPedToPlayerConversations::Update() {
    plugin::Call<0x43B0F0>();
}

// 0x43AB10
void CPedToPlayerConversations::EndConversation() {
    m_State           = eP2pState::INACTIVE;

    CAEPedSpeechAudioEntity::ReleasePlayerConversation();

    if (m_pPed) {
        m_pPed->EnablePedSpeech();
    }

    const auto player = FindPlayerPed(-1);
    if (g_ikChainMan.IsLooking(player)) {
        g_ikChainMan.AbortLookAt(player, 250);
    }

    if (m_pPed && g_ikChainMan.IsLooking(m_pPed)) {
        return g_ikChainMan.AbortLookAt(m_pPed, 250);
    }
}

// 0x43A7B0
void CConversations::Clear() {
    ZoneScoped;

    for (auto& conversation : m_Conversations) {
        conversation.Clear(true);
    }

    for (auto& node : m_Nodes) {
        node.Clear();
    }

    m_SettingUpConversation = 0;
    m_AwkwardSayStatus      = eAwkwardSayStatus::INACTIVE;
}

// 0x43C590
void CConversations::Update() {
    ZoneScoped;

    const auto updateConversations = [&]() {
        for (auto& conversation : m_Conversations) {
            conversation.Update();
        }
    };

    switch (m_AwkwardSayStatus) {
    case eAwkwardSayStatus::LOADING:
        if (AudioEngine.GetMissionAudioLoadingStatus(0) == 1) {
            AudioEngine.PlayLoadedMissionAudio(0);
            m_AwkwardSayStatus = eAwkwardSayStatus::PLAYING;
        }
        break;
    case eAwkwardSayStatus::PLAYING:
        if (AudioEngine.IsMissionAudioSampleFinished(0)) {
            m_AwkwardSayStatus = eAwkwardSayStatus::INACTIVE;
            updateConversations();
        }
        break;
    case eAwkwardSayStatus::INACTIVE:
        updateConversations();
        break;
    }
}

// 0x43A870
void CConversations::SetUpConversationNode(
    const char* questionKey,
    const char* answerYesKey,
    const char* answerNoKey,
    int32       questionWAV,
    int32       answerYesWAV,
    int32       answerNoWAV
) {
    auto& node = CConversations::m_aTempNodes[CConversations::m_SettingUpConversationNumNodes];
    strncpy(node.m_Name, questionKey, 6u);
    MakeUpperCase(node.m_Name);

    node.m_Speech  = questionWAV;
    node.m_SpeechY = answerYesWAV;
    node.m_SpeechN = answerNoWAV;

    if (answerYesKey) {
        strncpy(node.m_NameNodeYes, answerYesKey, 6u);
        MakeUpperCase(node.m_NameNodeYes);
    } else {
        node.m_NameNodeYes[0] = '\0';
    }
    if (answerNoKey) {
        strncpy(node.m_NameNodeNo, answerNoKey, 6u);
        MakeUpperCase(node.m_NameNodeNo);
    } else {
        node.m_NameNodeNo[0] = '\0';
    }
    ++CConversations::m_SettingUpConversationNumNodes;
}

// 0x43A960
void CConversations::RemoveConversationForPed(CPed* ped) {
    for (auto& conversation : m_Conversations) {
        if (conversation.m_pPed == ped) {
            conversation.Clear(false);
        }
    }
}

// 0x43B0B0
bool CConversations::IsPlayerInPositionForConversation(CPed* ped, bool randomConversation) {
    return FindConversationForPed(ped)->IsPlayerInPositionForConversation(randomConversation);
}

// 0x43AAC0
bool CConversations::IsConversationGoingOn() {
    for (const auto& conversation : m_Conversations) {
        if (conversation.m_Status != CConversationForPed::eStatus::INACTIVE) {
            return true;
        }
    }
    return false;
}

// 0x43B000
bool CConversations::IsConversationAtNode(const char* pName, CPed* pPed) {
    auto conversation = FindConversationForPed(pPed);
    assert(conversation);

    if (conversation->m_CurrentNode < 0 || conversation->m_Status == CConversationForPed::eStatus::PLAYER_SPEAKING) {
        return false;
    }
    // NOTSA - using stricmp instead of MakeUpperCase + strcmp
    return !stricmp(pName, CConversations::m_Nodes[conversation->m_CurrentNode].m_Name);
}

// 0x43A810
void CConversations::AwkwardSay(int32 whatToSay, CPed* speaker) {
    AudioEngine.PreloadMissionAudio(0, whatToSay);
    AudioEngine.AttachMissionAudioToPed(0, speaker);
    m_AwkwardSayStatus = eAwkwardSayStatus::LOADING;
}

// 0x43AA40
void CConversations::EnableConversation(CPed* ped, bool enabled) {
    FindConversationForPed(ped)->m_Enabled = enabled;
}

// 0x43A840
void CConversations::StartSettingUpConversation(CPed* ped) {
    m_SettingUpConversationPed = ped;
    ped->RegisterReference(m_SettingUpConversationPed);
    m_SettingUpConversationNumNodes = 0;
    m_SettingUpConversation         = true;
}

// 0x43ADB0
void CConversations::DoneSettingUpConversation(bool bSuppressSubtitles) {
    plugin::Call<0x43ADB0, bool>(bSuppressSubtitles);
}

inline void CConversationForPed::Clear(bool dontClearNodes) {
    if (!dontClearNodes) {
        assert(m_FirstNode >= 0);
        CConversations::m_Nodes[m_FirstNode].ClearRecursively();
    }
    m_FirstNode                 = -1;
    m_CurrentNode               = -1;
    m_pPed                      = nullptr;
    m_LastChange                = 0;
    m_LastTimeWeWereCloseEnough = 0;
}

// 0x43C190
void CConversationForPed::Update() {
    plugin::CallMethod<0x43C190, CConversationForPed*>(this);
}

inline CConversationForPed* CConversations::FindConversationForPed(CPed* ped) {
    for (auto& conversation : m_Conversations) {
        if (conversation.m_pPed == ped) {
            return &conversation;
        }
    }
    return nullptr;
}

// 0x43AC40
bool CConversationForPed::IsPlayerInPositionForConversation(bool randomConversation) {
    return plugin::CallMethodAndReturn<bool, 0x43AC40, CConversationForPed*, bool>(this, randomConversation);
}

inline void CConversationNode::Clear() {
    m_Name[0] = '\0';
    m_NodeYes = -1;
    m_NodeNo  = -1;
    m_Speech  = 0;
    m_SpeechY = 0;
    m_SpeechN = 0;
}

void CConversationNode::ClearRecursively() {
    if (m_NodeYes >= 0) {
        CConversations::m_Nodes[m_NodeYes].ClearRecursively();
    }
    if (m_NodeNo >= 0) {
        CConversations::m_Nodes[m_NodeNo].ClearRecursively();
    }
    Clear();
}
