#include "StdInc.h"

HANDLE(&gStreamFileHandles)[MAX_CD_STREAM_HANDLES] = *(HANDLE(*)[MAX_CD_STREAM_HANDLES])0x8E4010;
char(&gCdImageNames)[MAX_CD_STREAM_HANDLES][MAX_CD_STREAM_IMAGE_NAME_SIZE] = *(char(*)[MAX_CD_STREAM_HANDLES][MAX_CD_STREAM_IMAGE_NAME_SIZE])0x8E4098;
uint32& gStreamFileCreateFlags = *(uint32*)0x8E3FE0;
CdStream*& gCdStreams = *(CdStream**)0x8E3FFC;
int32& gStreamCount = *(int32*)0x8E4090;
int32& gOpenStreamCount = *(int32*)0x8E4094;
int32& gStreamingInitialized = *(int32*)0x8E3FE4;
int32& gOverlappedIO = *(int32*)0x8E3FE8;

// Queue of the streams
// The streamer thread processes based on it
// Size of `gStreamCount + 1`
Queue& gStreamQueue = *(Queue*)0x8E3FEC;
HANDLE& gStreamSemaphore = *(HANDLE*)0x8E4004;
HANDLE& gStreamingThread = *(HANDLE*)0x8E4008;
DWORD& gStreamingThreadId = *(DWORD*)0x8E4000;
uint32& gLastCdStreamPosn = *(uint32*)0x8E4898;

#define APPLY_CD_STREAM_DEADLOCK_FIX 1

#ifdef APPLY_CD_STREAM_DEADLOCK_FIX
// thanks to http://forums.codeguru.com/showthread.php?175474-a-CCriticalSection-question
class CSync {
public:
    CSync() { InitializeCriticalSection(&m_CriticalSection); }
    ~CSync() { DeleteCriticalSection(&m_CriticalSection); }
    void Acquire() { EnterCriticalSection(&m_CriticalSection); }
    void Release() { LeaveCriticalSection(&m_CriticalSection); }

private:
    CRITICAL_SECTION m_CriticalSection;
};

class CLockGuard {
public:
    CLockGuard(CSync& refSync) : m_refSync(refSync) { Lock(); }
    ~CLockGuard() { Unlock(); }

private:
    CSync& m_refSync;
    CLockGuard(const CLockGuard& refcSource);
    CLockGuard& operator=(const CLockGuard& refcSource);

public:
    void Lock() { m_refSync.Acquire(); }
    void Unlock() { m_refSync.Release(); }
};

static CSync cdStreamThreadSync;
#endif

void InjectCdStreamHooks() {
    RH_ScopedNamespaceName("CdStream");
    RH_ScopedCategoryGlobal();

    RH_ScopedGlobalInstall(CdStreamOpen, 0x4067B0);
    RH_ScopedGlobalInstall(CdStreamSync, 0x406460);
    RH_ScopedGlobalInstall(CdStreamGetStatus, 0x4063E0);
    RH_ScopedGlobalInstall(CdStreamRead, 0x406A20);
    RH_ScopedGlobalInstall(CdStreamThread, 0x406560);
    RH_ScopedGlobalInstall(CdStreamInitThread, 0x4068F0);
    RH_ScopedGlobalInstall(CdStreamInit, 0x406B70);
    RH_ScopedGlobalInstall(CdStreamRemoveImages, 0x406690);
    RH_ScopedGlobalInstall(CdStreamShutdown, 0x406370);
}

// NOTSA
int32 CdStreamFindFreeStream() {
    for (int32 i = 0; i < MAX_CD_STREAM_HANDLES; i++) {
        if (!gStreamFileHandles[i]) {
            return i;
        }
    }
    return -1;
}

// 0x4067B0
int32 CdStreamOpen(const char* path) {
    const auto idx = CdStreamFindFreeStream();
    assert(idx != -1);

    // Open the file
    SetLastError(NO_ERROR);
    const auto file = gStreamFileHandles[idx] = CreateFileA(
        path,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        gStreamFileCreateFlags | FILE_ATTRIBUTE_READONLY | FILE_FLAG_RANDOM_ACCESS,
        nullptr
    );
    if (file == INVALID_HANDLE_VALUE) {
        return 0;
    }

    strncpy_s(gCdImageNames[idx], path, MAX_CD_STREAM_IMAGE_NAME_SIZE);

    return idx << 24;
}

// This function halts the caller thread if CdStreamThread is still reading the file to "sync" it.
// CdStreamSync is always called from the main thread. It's called when entering or exiting
// interiors. It's used when the game wants a model to be loaded immediately,
// like when spawning after exiting interiors.
// This function was responsible for causing the deadlock since it was not thread-safe.
// The deadlock problem only exists on PC version of SA. R* fixed it on android.
// So it's fixed here because we used a critical section for it.
// Workflow:
// 1. CdStreamRead is called on the main thread and signals gStreamSemaphore to
//    read game asset in a secondary thread a.k.a. CdStreamThread.
// 2. CdStreamThread starts reading the file on the secondary thread, and the main thread
//    immediately calls CdStreamSync to stop the main thread from execution until the model data has been read from the file.
// 3. When CdStreamThread is done reading the file, it signals `stream.sync.hSemaphore`, so the main thread can
//    continue executing code and continue the gameplay.
// 0x406460
eCdStreamStatus CdStreamSync(int32 streamId) {
    CdStream& stream = gCdStreams[streamId];

#ifdef APPLY_CD_STREAM_DEADLOCK_FIX
    CLockGuard lockGuard(cdStreamThreadSync);
#endif

    if (gStreamingInitialized) {
        if (stream.nSectorsToRead) {
            stream.bLocked = true;
#ifdef APPLY_CD_STREAM_DEADLOCK_FIX
            lockGuard.Unlock();
#endif
            WaitForSingleObject(stream.sync.hSemaphore, INFINITE);
#ifdef APPLY_CD_STREAM_DEADLOCK_FIX
            lockGuard.Lock();
#endif
        }
        stream.bInUse = false;
        return stream.status;
    }
    else if (gOverlappedIO && stream.hFile) {
        DWORD numberOfBytesTransferred = 0;
        if (!GetOverlappedResult(stream.hFile, &stream.overlapped, &numberOfBytesTransferred, true)) {
            return eCdStreamStatus::READING_FAILURE;
        }
    }
    return eCdStreamStatus::READING_SUCCESS;
}

// 0x4063E0
eCdStreamStatus CdStreamGetStatus(int32 streamId) {
    CdStream& stream = gCdStreams[streamId];
    if (gStreamingInitialized) {
        if (stream.bInUse)
            return eCdStreamStatus::READING;

        if (stream.nSectorsToRead)
            return eCdStreamStatus::WAITING_TO_READ;

        if (stream.status != eCdStreamStatus::READING_SUCCESS) {
            const eCdStreamStatus status = stream.status;
            stream.status = eCdStreamStatus::READING_SUCCESS;
            return status;
        }
    }
    else if (gOverlappedIO) {
        if (WaitForSingleObjectEx(stream.hFile, 0, 1) != WAIT_OBJECT_0)
            return eCdStreamStatus::READING;
    }
    return eCdStreamStatus::READING_SUCCESS;
}

// When CdStreamRead is called, it will update CdStream information for the channel and
// release gStreamSemaphore, so the secondary thread `CdStreamThread` can start reading the models.
// If this function is called with the same channelId/streamId whilst CdStreamThread is still reading the previous model
// for the channel, then it will return false.
// When CdStreamThread is done reading the model, then CdStreamThread will set `stream.nSectorsToRead` and `stream.bInUse` to 0,
// so the main thread can call CdStreamRead again to read more models.
// 0x406A20
bool CdStreamRead(int32 streamId, void* lpBuffer, uint32 offsetAndHandle, int32 sectorCount) {
    CdStream& stream = gCdStreams[streamId];

    gLastCdStreamPosn = sectorCount + offsetAndHandle;

    const auto sectorOffset = offsetAndHandle & 0xFFFFFF;
    stream.hFile = gStreamFileHandles[offsetAndHandle >> 24];

    SetLastError(NO_ERROR);

    if (gStreamingInitialized) {
        if (stream.nSectorsToRead || stream.bInUse) { // Already reading, so do nothing [NOTE: Why the fuck is the file handle above modified in this case?]
            return false;
        }
        stream.status = eCdStreamStatus::READING_SUCCESS;
        stream.nSectorOffset = sectorOffset;
        stream.nSectorsToRead = sectorCount;
        stream.lpBuffer = lpBuffer;
        stream.bLocked = false;
        AddToQueue(&gStreamQueue, streamId); // Queue this stream to be processed by the streaming thread
        VERIFY(ReleaseSemaphore(gStreamSemaphore, 1, nullptr)); // Initiate the streamer thread now
        return true;
    }
    else { // Otherwise, read on this thread [Though, we still might use overlapped IO]
        const auto toReadBytes = sectorCount * STREAMING_SECTOR_SIZE;
        const auto offsetBytes = sectorOffset * STREAMING_SECTOR_SIZE;
        if (gOverlappedIO) {
            const auto overlapped = &gCdStreams[streamId].overlapped;
            overlapped->Offset = offsetBytes;
            return ReadFile(stream.hFile, lpBuffer, toReadBytes, nullptr, overlapped) || GetLastError() == ERROR_IO_PENDING; // IO pending just means that we still haven't finished reading the overlapped
        }
        else {
            SetFilePointer(stream.hFile, offsetBytes, nullptr, 0);
            return ReadFile(stream.hFile, lpBuffer, toReadBytes, nullptr, nullptr);
        }
    }
}

// 0x406560
// The main streamer/reader thread.
// Workflow:
// 1. Wait for `gStreamSemaphore` to be released
// 2. Pop the next stream in `gStreamQueue`
// 3. Process it [Wait for `ReadFile` to finish]
[[noreturn]] void WINAPI CdStreamThread(LPVOID lpParam) {
    while (true) {
        // Wait for the sema to be released
        WaitForSingleObject(gStreamSemaphore, INFINITE);

        // Get next stream in the queue
        auto& stream = gCdStreams[GetFirstInQueue(&gStreamQueue)];

        // We're currently reading from it, so mark it as such
        stream.bInUse = true;

        // Now, if the previous state was good, read, otherwise don't do anything
        if (stream.status == eCdStreamStatus::READING_SUCCESS) {
            const DWORD toReadBytes = stream.nSectorsToRead * STREAMING_SECTOR_SIZE;
            const DWORD offsetBytes = stream.nSectorOffset * STREAMING_SECTOR_SIZE;
            stream.status = [&]() -> bool {
                if (gOverlappedIO) {
                    stream.overlapped.Offset = offsetBytes;

                    // Try reading the overlapped
                    if (!ReadFile(stream.hFile, stream.lpBuffer, toReadBytes, nullptr, &stream.overlapped) && GetLastError() != ERROR_IO_PENDING) {
                        return false;
                    }

                    // There's an IO pending, so wait for our it/them to complete
                    if (DWORD readBytes = 0; !GetOverlappedResult(stream.hFile, &stream.overlapped, &readBytes, true)) {
                        return false;
                    }

                    // We're good
                    return true;
                } else { // Not using overlapped, so read directly
                    return SetFilePointerEx(stream.hFile, LARGE_INTEGER{ offsetBytes }, nullptr, FILE_BEGIN) // NOTSA: Using `Ex` function instead
                        && ReadFile(stream.hFile, stream.lpBuffer, toReadBytes, nullptr, nullptr);
                }
            }() ? eCdStreamStatus::READING_SUCCESS : eCdStreamStatus::READING_FAILURE;
        }

        // Stream processed, so remove it from the queue
        RemoveFirstInQueue(&gStreamQueue);

#ifdef APPLY_CD_STREAM_DEADLOCK_FIX
        CLockGuard lockGuard(cdStreamThreadSync);
#endif
        // locking is necessary here, so ReleaseSemaphore is not called before WaitForSingleObject
        // in CdStreamSync to avoid causing a deadlock.
        stream.nSectorsToRead = 0;
        if (stream.bLocked)
            ReleaseSemaphore(stream.sync.hSemaphore, 1, nullptr);
        stream.bInUse = false;
    }
}

// 0x4068F0
void CdStreamInitThread() {
    SetLastError(NO_ERROR);

    for (auto& stream : std::span{ gCdStreams, (size_t)gStreamCount }) {
        HANDLE hSemaphore = OS_SemaphoreCreate(2, nullptr);
        stream.sync.hSemaphore = hSemaphore;
        if (!hSemaphore) {
            DEV_LOG("cdvd_stream: failed to create sync semaphore");
            return;
        }
    }

    InitialiseQueue(&gStreamQueue, gStreamCount + 1);

    gStreamSemaphore = OS_SemaphoreCreate(5, "CdStream");
    if (!(gStreamSemaphore = OS_SemaphoreCreate(5, "CdStream"))) {
        DEV_LOG("cdvd_stream: failed to create stream semaphore");
        return;
    }

    if (!(gStreamingThread = CreateThread(
        nullptr,
        0x10000,
        (LPTHREAD_START_ROUTINE)CdStreamThread,
        nullptr,
        CREATE_SUSPENDED,
        &gStreamingThreadId))
        ) {
        DEV_LOG("cdvd_stream: failed to create streaming thread");
        return;
    }

    SetThreadPriority(gStreamingThread, GetThreadPriority(GetCurrentThread()));
    ResumeThread(gStreamingThread);
}

// 0x406B70
void CdStreamInit(int32 streamCount) {
    for (int32 i = 0; i < MAX_CD_STREAM_HANDLES; i++) {
        gStreamFileHandles[i] = nullptr;
        gCdImageNames[i][0] = 0;
    }
    DWORD bytesPerSector = 0;
    DWORD totalNumberOfClusters = 0;
    DWORD numberOfFreeClusters = 0;
    DWORD sectorsPerCluster = 0;
    GetDiskFreeSpace(nullptr, &sectorsPerCluster, &bytesPerSector, &numberOfFreeClusters, &totalNumberOfClusters);

    gStreamFileCreateFlags = FILE_FLAG_OVERLAPPED;
#ifndef FIX_BUGS // this just slows down streaming
    if (bytesPerSector <= STREAMING_SECTOR_SIZE) {
        gStreamFileCreateFlags |= FILE_FLAG_NO_BUFFERING;
    }
#endif

    gOverlappedIO = 1;
    gStreamingInitialized = 0;
    auto* pAllocatedMemory = CMemoryMgr::MallocAlign(STREAMING_SECTOR_SIZE, bytesPerSector);
    SetLastError(NO_ERROR);
    gOpenStreamCount = 0;
    gStreamCount = streamCount;
    gCdStreams = (CdStream*)LocalAlloc(LPTR, sizeof(CdStream) * streamCount);
    CdStreamOpen("MODELS\\GTA3.IMG");
    bool bStreamRead = CdStreamRead(0, pAllocatedMemory, 0, 1);
    CdStreamRemoveImages();
    gStreamingInitialized = 1;
    if (!bStreamRead) {
        gOverlappedIO = 0;
        gStreamFileCreateFlags &= ~FILE_FLAG_OVERLAPPED;
    }
    CdStreamInitThread();
    CMemoryMgr::FreeAlign(pAllocatedMemory);
}

// 0x406690
void CdStreamRemoveImages() {
    for (int32 i = 0; i < gStreamCount; ++i) {
        CdStreamSync(i);
    }
    for (int32 i = 0; i < gOpenStreamCount; i++) {
        SetLastError(NO_ERROR);
        if (gStreamFileHandles[i]) {
            CloseHandle(gStreamFileHandles[i]);
        }
        gStreamFileHandles[i] = nullptr;
        gCdImageNames[i][0] = 0;
    }
    gOpenStreamCount = 0;
}

// 0x406370
void CdStreamShutdown() {
    if (gStreamingInitialized) {
        FinalizeQueue(&gStreamQueue);
        CloseHandle(gStreamSemaphore);
        CloseHandle(gStreamingThread);
        for (auto& stream : std::span{ gCdStreams, (size_t)gStreamCount }) {
            CloseHandle(stream.sync.hSemaphore);
        }
    }
    LocalFree(gCdStreams);
}
