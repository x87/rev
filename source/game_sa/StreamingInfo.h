/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

enum eStreamingFlags {
    STREAMING_UNKNOWN_1 = 0x1,
    STREAMING_GAME_REQUIRED = 0x2,
    STREAMING_MISSION_REQUIRED = 0x4,
    STREAMING_KEEP_IN_MEMORY = 0x8,
    STREAMING_PRIORITY_REQUEST = 0x10,
    STREAMING_LOADING_SCENE = 0x20,
    STREAMING_DONTREMOVE_IN_LOADSCENE = STREAMING_LOADING_SCENE | STREAMING_PRIORITY_REQUEST | STREAMING_KEEP_IN_MEMORY | STREAMING_MISSION_REQUIRED | STREAMING_GAME_REQUIRED,
};

enum class eStreamingLoadState {
    // Model not yet loaded
    NOT_LOADED,

    // Model loaded currently
    LOADED,

    // In case of big models: first half has been loaded, 2nd half is being read
    // Currently unused
    FINISHING,

    // Both `REQUESTED` and `READING` can be cancelled by setting the state to `NOT_LOADED`

    // In request list
    REQUESTED,

    // Currently being read 
    READING,

    // Reading finished, waiting to be loaded by main thread
    READING_FINISHED,
};

//constexpr auto STREAMING_SECTOR_SIZE = 2048u;

class CStreamingInfo {
public:
    //class Iterator {
    //public:
    //    using iterator_category = std::bidirectional_iterator_tag;
    //    using difference_type   = std::ptrdiff_t;
    //    using value_type        = CStreamingInfo;
    //    using pointer           = CStreamingInfo*;
    //    using reference         = CStreamingInfo&;

    //    Iterator(pointer ptr) :
    //        m_item(ptr)
    //    {}

    //    reference operator*() const { return *m_ptr; }
    //    pointer operator->() { return m_ptr; }
    //    Iterator& operator++() {
    //        m_ptr = 
    //        return *this;
    //    }
    //    Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
    //    friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
    //    friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
    //private:
    //    CStreamingInfo* m_ptr{};
    //};

public:
    int16 m_nNextIndex;     // ms_pArrayBase array index
    int16 m_nPrevIndex;     // ms_pArrayBase array index
    int16 m_nNextIndexOnCd; // ModelId after this file in the containing image file
    union {
        uint8 m_nFlags; // see eStreamingFlags
        struct {
            uint8 bUnkn0x1 : 1;
            uint8 bGameRequired : 1;
            uint8 bMissionRequired : 1;
            uint8 bKeepInMemory : 1;
            uint8 bPriorityRequest : 1;
            uint8 bLoadingScene : 1;
        };
    };
    uint8  m_nImgId;        // Index into CStreaming::ms_files
    uint32 m_nCdPosn;       // Position in directory (in sectors)
    uint32 m_nCdSize;       // Size of resource (in sectors); m_nCdSize * STREAMING_BLOCK_SIZE = actual size in bytes

    std::atomic<eStreamingLoadState> m_nLoadState;

    static CStreamingInfo*& ms_pArrayBase;

public:
    static void InjectHooks();

    void Init();
    void AddToList(CStreamingInfo* listStart);
    uint32 GetCdPosn();
    void SetCdPosnAndSize(uint32 CdPosn, uint32 CdSize);
    bool GetCdPosnAndSize(uint32& CdPosn, uint32& CdSize);
    bool HasCdPosnAndSize() const noexcept { return m_nCdSize != 0; }
    uint32 GetCdSize() { return m_nCdSize; }
    CStreamingInfo* GetNext() { return m_nNextIndex == -1 ? nullptr : &ms_pArrayBase[m_nNextIndex]; }
    CStreamingInfo* GetPrev() { return m_nPrevIndex == -1 ? nullptr : &ms_pArrayBase[m_nPrevIndex]; }
    bool InList();
    void RemoveFromList();
    auto GetNextOnCd() const noexcept { return (uint32_t)m_nNextIndexOnCd; }
    auto GetImgId() const noexcept { return m_nImgId; }

    void SetFlags(uint32 flags) { m_nFlags |= flags; }
    void ClearFlags(uint32 flags) { m_nFlags &= ~flags; }
    auto GetFlags() const noexcept { return m_nFlags; }
    void ClearAllFlags() noexcept { m_nFlags = 0; } // Clears all flags
    bool AreAnyFlagsSetOutOf(uint32 flags) const noexcept { return GetFlags() & flags; } // Checks if any flags in `flags` are set

    auto GetLoadState() const noexcept { return m_nLoadState.load(); }

    bool IsLoadedOrBeingRead() const noexcept {
        switch (GetLoadState()) {
        case eStreamingLoadState::LOADED:
        case eStreamingLoadState::READING:
            return true;
        default:
            return false;
        }
    }

    bool IsLoaded() const { return GetLoadState() == eStreamingLoadState::LOADED; }
    bool IsRequested() const { return GetLoadState() == eStreamingLoadState::REQUESTED; }
    bool IsBeingRead() const { return GetLoadState() == eStreamingLoadState::READING; }
    bool IsLoadingFinishing() const { return GetLoadState() == eStreamingLoadState::FINISHING; }

    void SetLoadState(eStreamingLoadState st) noexcept { m_nLoadState = st; }

    bool DontRemoveInLoadScene() const noexcept { return GetFlags() & eStreamingFlags::STREAMING_DONTREMOVE_IN_LOADSCENE; }
    bool IsGameRequired() const noexcept { return GetFlags() & eStreamingFlags::STREAMING_GAME_REQUIRED; }
    bool IsMissionRequired() const noexcept { return GetFlags() & eStreamingFlags::STREAMING_MISSION_REQUIRED; }
    bool DoKeepInMemory() const noexcept { return GetFlags() & eStreamingFlags::STREAMING_KEEP_IN_MEMORY; }
    bool IsPriorityRequest() const noexcept { return GetFlags() & eStreamingFlags::STREAMING_PRIORITY_REQUEST; }
    bool IsLoadingScene() const noexcept { return GetFlags() & eStreamingFlags::STREAMING_LOADING_SCENE; }
    bool IsRequiredToBeKept() const noexcept { return IsGameRequired() || IsMissionRequired() || DoKeepInMemory(); } // GameRequired || MissionRequired || KeepInMemory
    bool IsMissionOrGameRequired() const noexcept { return IsMissionRequired() || IsGameRequired(); } // TODO: FIX
};
VALIDATE_SIZE(CStreamingInfo, 0x14);
