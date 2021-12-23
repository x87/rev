#include "StdInc.h"
#include <filesystem>
#include <ranges>
#include <vector>

#include "ResourceStreamer.h"
#include "StreamingChannel.h"

namespace fs = std::filesystem;
namespace rng = std::ranges;

static std::vector<StreamingChannel> s_channels;
static std::vector<fs::path>         s_directoryNames;


namespace ResourceStreamer{
void ProcessDirectory(const fs::path& path, uint32 archiveId);


auto Init(uint32 numChannels) -> void {
    assert(s_channels.empty());

    s_channels.resize(numChannels); // Make channels

    /*for (auto i : rng::views::iota(numChannels)) {
        s_channels.push_back({});
    }*/

    uint32 n{};
    for (auto& dir : s_directoryNames) {
        for (auto& ch : s_channels) {
            ch.AddDirectory(dir);
        }
        ProcessDirectory(dir, n++);
    }

    // Set buffers for each channel
    const auto perChannelSz = s_bufferSz * 3;
    auto buffer = (Sector*)CMemoryMgr::MallocAlign(perChannelSz * STREAMING_SECTOR_SIZE * numChannels, STREAMING_SECTOR_SIZE);
    for (auto& ch : s_channels) {
        ch.SetBuffer(buffer, buffer + perChannelSz);
        buffer += perChannelSz;
    }
    printf(">> Buffer size: %u (%u K) \n", perChannelSz, perChannelSz * STREAMING_SECTOR_SIZE);
}

void ProcessDirectory(const fs::path& path, uint32 archiveId) {
    SetLastError(0);
    HANDLE f = CreateFileW(
        path.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );
    assert(!GetLastError());

    const auto Read = [f](auto buff, auto bytes) {
        ReadFile(f, buff, bytes, NULL, NULL);
    };

    // Read and check IMG file version
    {
        char version[4];
        Read(&version, 4u); // `VER2` for SA
        assert(strncmp(version, "VER2", sizeof(version)) == 0); // NOTSA
    }

    int32 previousModelId = -1;
    int32 entryCount;
    Read(&entryCount, 4u);
    for (int32 i = 0; i < entryCount; i++) {
        CDirectory::DirectoryInfo entry{};
        Read(&entry, sizeof(CDirectory::DirectoryInfo));

        // Maybe increase buffer size
        s_bufferSz = std::max(s_bufferSz, (uint32)entry.m_nStreamingSize);

        // Find extension from name
        constexpr auto nameSize = sizeof(CDirectory::DirectoryInfo::m_szName);
        entry.m_szName[nameSize - 1] = 0;
        char* pExtension = strchr(entry.m_szName, '.');
        if (!pExtension
            || (size_t)(pExtension - entry.m_szName) > nameSize - 4u // Check if extension string length is  < 4
            ) {
            entry.m_szName[nameSize - 1] = 0;
            previousModelId = -1;
            continue;
        }

        // Replace `.` with a null terminator
        // This way m_szName only contains the file name without the extension
        // Eg.: `car.dff` becomes `car`
        *pExtension = 0;

        // TODO: Maybe use `makeFourCC` here + switch case

        const auto ExtensionIs = [=](const char what[]) {
            return _memicmp(pExtension + 1, what, strlen(what)) == 0;
        };

        int32 modelId = -1;
        if (ExtensionIs("DFF")) {
            if (!CModelInfo::GetModelInfo(entry.m_szName, &modelId)) {
                entry.m_nOffset |= archiveId << 24;
                CStreaming::ms_pExtraObjectsDir->AddItem(entry);
                previousModelId = -1;
                continue;
            }

        }
        else if (ExtensionIs("TXD")) {
            int32 txdSlot = CTxdStore::FindTxdSlot(entry.m_szName);
            if (txdSlot == -1) {
                txdSlot = CTxdStore::AddTxdSlot(entry.m_szName);
                CVehicleModelInfo::AssignRemapTxd(entry.m_szName, txdSlot);
            }
            modelId = TXDToModelId(txdSlot);

        }
        else if (ExtensionIs("COL")) {
            int32 colSlot = CColStore::FindColSlot();
            if (colSlot == -1)
                colSlot = CColStore::AddColSlot(entry.m_szName);
            modelId = COLToModelId(colSlot);

        }
        else if (ExtensionIs("IPL")) {
            int32 iplSlot = CIplStore::FindIplSlot(entry.m_szName);
            if (iplSlot == -1)
                iplSlot = CIplStore::AddIplSlot(entry.m_szName);
            modelId = IPLToModelId(iplSlot);

        }
        else if (ExtensionIs("DAT")) {
            // Extract nodes file sector from name (Remember the format: `nodesXX.dat` where XX is the id)
            // TODO: Maybe one day add some error here if sscanf doesn't return 1 (that is, the number couldn't be read)
            (void)sscanf(&entry.m_szName[sizeof("nodes") - 1], "%d", &modelId);
            modelId += RESOURCE_ID_DAT;

        }
        else if (ExtensionIs("IFP")) {
            modelId = IFPToModelId(CAnimManager::RegisterAnimBlock(entry.m_szName));

        }
        else if (ExtensionIs("RRR")) {
            modelId = RRRToModelId(CVehicleRecording::RegisterRecordingFile(entry.m_szName));

        }
        else if (ExtensionIs("SCM")) {
            modelId = SCMToModelId(CTheScripts::StreamedScripts.RegisterScript(entry.m_szName));
        }
        else {
            *pExtension = '.'; // Put `.` back as previously it was replace with a null terminator
            previousModelId = -1;
            continue;
        }

        CStreamingInfo& info = CStreaming::GetInfo(modelId);
        if (!info.HasCdPosnAndSize()) {
            info.m_nImgId = archiveId;

            if (entry.m_nSizeInArchive)
                entry.m_nStreamingSize = entry.m_nSizeInArchive;

            info.SetCdPosnAndSize(entry.m_nOffset, entry.m_nStreamingSize);
            info.ClearAllFlags();

            if (previousModelId != -1)
                info.m_nNextIndexOnCd = modelId;

            previousModelId = modelId;
        }
    }

    CloseHandle(f);
}

auto AddDirectory(const char* path) -> uint32 {
    const auto& dir = s_directoryNames.emplace_back(path);
    const auto idx = static_cast<uint32_t>(&dir - &s_directoryNames.front());

    /*ProcessDirectory(dir, idx);

    for (auto& ch : s_channels) {
        ch.AddDirectory(dir);
    }*/

    return idx;
}

auto RequestResource(CStreamingInfo& info) -> void {
    // Insert request
    {
        std::lock_guard guard{ s_mtx };
        const auto it = rng::lower_bound(s_requests | rng::views::reverse, &info,
            [](const auto a, const auto b) -> bool
            {
                if (a->IsPriorityRequest() == b->IsPriorityRequest()) {
                    if (a->GetNextOnCd() == -1 && b->GetNextOnCd() == -1)
                        return false; // Unlikely, but possible
                    return b->GetNextOnCd() == -1; // Always prefer files with `NextOnCd` set
                }
                return !a->IsPriorityRequest();
            }
        );
        s_requests.emplace(it.base(), &info);

        // Notify a workers
        s_cv.notify_all();
    }
}

void Sync() {
    std::lock_guard guard{ s_mtx };
    if (!s_requests.empty())
        s_cv.notify_all();
}

bool HasFinished() {
    std::lock_guard guard{ s_mtx };
    return s_requests.empty() && !s_numBeingRead;
}

auto GetReadResource(ReadResourceStackCtx& ctx) -> std::optional<ReadResourceInfo> {
    size_t chIdx{};
    for (auto& ch : s_channels) {
        if (const auto item = ch.GetReadItem()) {
            const auto [rri, resIdx] = *item;
            ctx = { chIdx, resIdx };
            return rri;
        }
        chIdx++;
    }
    return std::nullopt;
}

void PopReadResoure(ReadResourceStackCtx ctx) {
    s_channels[ctx.chIdx].PopReadItem(ctx.resIdx);
}
};
