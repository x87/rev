#pragma once

class CPed;

enum {
    MAX_NUM_CONVERSATIONS = 14,
    MAX_NUM_CONVERSATION_NODES = 50,
};


class CPedToPlayerConversations {
public:
    enum class eP2pState : uint32 {
        INACTIVE = 0,
        PEDHASOPENED,
        WAITINGFORFINALWORD,
        WAITINGTOFINISH,
    };

    static inline eP2pState& m_State                        = *(eP2pState*)0x969A20;
    static inline CPed*&     m_pPed                         = *(CPed**)0x9691C0;
    static inline int32&     m_Topic                        = *(int32*)0x9691BC;
    static inline uint32&    m_TimeOfLastPlayerConversation = *(uint32*)0x9691B4;
    static inline uint32&    m_StartTime                    = *(uint32*)0x9691B8;
    static inline bool&      m_bPositiveReply               = *(bool*)0x9691B0; // unused
    static inline bool&      m_bPositiveOpening             = *(bool*)0x9691B1;

    static void Clear();
    static void Update();
    static void EndConversation();
};

class CConversationNode {
public:
    char  m_Name[8];
    int16 m_NodeYes;
    int16 m_NodeNo;
    int32 m_Speech;
    int32 m_SpeechY;
    int32 m_SpeechN;

    inline void Clear();
    inline void ClearRecursively();
};

VALIDATE_SIZE(CConversationNode, 0x18);

class CConversationForPed {
public:
    enum eStatus : uint32 {
        INACTIVE = 0,
        PLAYER_SPEAKING,
        PED_SPEAKING,
        WAITINGFORINPUT
    };

    int32   m_FirstNode;
    int32   m_CurrentNode;
    CPed*   m_pPed;
    uint32  m_LastChange;
    uint32  m_LastTimeWeWereCloseEnough;
    eStatus m_Status;
    bool    m_Enabled;
    bool    m_SuppressSubtitles;

    inline void Clear(bool dontClearNodes);
    void        Update();
    bool        IsPlayerInPositionForConversation(bool randomConversation);
};

VALIDATE_SIZE(CConversationForPed, 0x1C);

class CConversations {
public:
    enum class eAwkwardSayStatus : int32 {
        INACTIVE = 0,
        LOADING  = 1,
        PLAYING  = 2,
    };

    static inline eAwkwardSayStatus& m_AwkwardSayStatus      = *(eAwkwardSayStatus*)0x9691C4;
    static inline bool&              m_SettingUpConversation = *(bool*)0x9691D0;
    static inline auto&              m_Conversations         = *(std::array<CConversationForPed, MAX_NUM_CONVERSATIONS>*)0x009691EC;
    static inline auto&              m_Nodes                 = *(std::array<CConversationNode, MAX_NUM_CONVERSATION_NODES>*)0x969570;

    static void Clear();
    static void RemoveConversationForPed(CPed* ped);

    static void                        Update();
    static void                        SetUpConversationNode(const char*, const char*, const char*, int32, int32, int32);
    static bool                        IsPlayerInPositionForConversation(CPed* ped, bool randomConversation);
    static bool                        IsConversationGoingOn();
    static inline CConversationForPed* FindConversationForPed(CPed* ped);
    static bool                        IsConversationAtNode(char* pName, CPed* pPed);
    static void                        AwkwardSay(int32 whatToSay, CPed* speaker);
    /*
    static void GetConversationStatus(CPed* ped);
    static void EnableConversation(CPed* ped, bool);
    static void DoneSettingUpConversation(bool);
    static void FindFreeNodeSlot();
    static void FindFreeConversationSlot();
    static void StartSettingUpConversation(CPed* ped);
    
     */
};
