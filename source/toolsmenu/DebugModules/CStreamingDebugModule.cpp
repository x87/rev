#include "StdInc.h"

#include <extensions/utility.hpp>
#include "CStreamingDebugModule.h"
#include "Streaming.h"

using namespace ImGui;

void DrawChannelStates() {
    if (!BeginTable("Loading Channel States", 6, ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Borders)) {
        return;
    }

    TableSetupColumn("ID",               0);
    TableSetupColumn("Status",           0);
    TableSetupColumn("#Sectors",         0);
    TableSetupColumn("#Tries",           0);
    TableSetupColumn("Cd Stream Status", 0);
    TableSetupColumn("Model IDs");
    
    TableHeadersRow();

    for (auto i = 0; i < 2; i++) {
        notsa::ui::ScopedID columnIDScope{ i };

        const auto& channel = CStreaming::ms_channel[i];

        // ID
        TableNextColumn();
        Text("%d", i);

        // Status
        TableNextColumn();
        TextUnformatted([&] {
            switch (channel.LoadStatus) {
            case eChannelState::IDLE:    return "IDLE";
            case eChannelState::ERR:     return "ERR";
            case eChannelState::STARTED: return "STARTED";
            case eChannelState::READING: return "READING";
            default:                     return "Unknown";
            }
        }());

        // #Sectors
        TableNextColumn();
        Text("%d", channel.sectorCount);

        // #Tries
        TableNextColumn();
        Text("%d", channel.totalTries);

        // Cd Stream Status
        TableNextColumn();
        TextUnformatted([&] {
            switch (channel.m_nCdStreamStatus) {
            case eCdStreamStatus::READING_SUCCESS: return "READING_SUCCESS";
            case eCdStreamStatus::WAITING_TO_READ: return "WAITING_TO_READ";
            case eCdStreamStatus::READING_FAILURE: return "READING_FAILURE";
            case eCdStreamStatus::READING:         return "READING";
            default:                               return "UNKNOWN";
            }
        }());

        // Model IDs
        TableNextColumn();
        for (auto modelId : channel.modelIds) {
            Text("%d", modelId);
            SameLine();
        }
    }

    EndTable();
}

void DrawListSizes() {
    constexpr auto GetListSize = [](auto begin, auto end) {
        uint32 n{};
        for (auto it = begin->GetNext(); it != end; it = it->GetNext())
            n++;
        return n;
    };
    Text("Req. list size: %u", GetListSize(CStreaming::ms_pStartRequestedList, CStreaming::ms_pEndRequestedList));
    Text("Loaded list size: %u", GetListSize(CStreaming::ms_startLoadedList, CStreaming::ms_pEndLoadedList));
}

void CStreamingDebugModule::RenderWindow() {
    notsa::ui::ScopedWindow window{ "Streaming", {700, 200.f}, m_IsOpen };
    if (!m_IsOpen) {
        return;
    }

    if (Button("ReInit")) {
        CStreaming::ReInit();
    }

    if (Button("Speed Test")) {
        using namespace std::chrono;
        using Clock = high_resolution_clock;
        {
            notsa::TimedScope<Clock> ts{};

            uint32 cnt = 0;
            for (auto i = 1000; i < 18000; i++) {
                if (!CModelInfo::GetModelInfo(i)) {
                    continue;
                }
                CStreaming::RequestModel(i, STREAMING_KEEP_IN_MEMORY | STREAMING_MISSION_REQUIRED | STREAMING_GAME_REQUIRED);
                cnt++;
            }
            DEV_LOG("Requested models ({}) in {}", cnt, ts.DurationNow());
        }
        if (false) {
            notsa::TimedScope<Clock> ts{};
            CStreaming::LoadAllRequestedModels(false);
            DEV_LOG("LoadAllRequestedModels in {}", ts.DurationNow());
        }
    }

    Text("Memory: %u/%u KiB", CStreaming::ms_memoryUsedBytes / 1024, CStreaming::ms_memoryAvailable / 1024);
    Text("Buffer size: %u Sectors (%u KiB)", CStreaming::GetWholeBufferSize(), CStreaming::GetWholeBufferSize() * STREAMING_SECTOR_SIZE / 1024);

    DrawListSizes();
    DrawChannelStates();

    Text("Loading big model: %i", (int32)CStreaming::ms_bLoadingBigModel);
}

void CStreamingDebugModule::RenderMenuEntry() {
    if (ImGui::BeginMenu("Stats")) {
        ImGui::MenuItem("Streaming", NULL, &m_IsOpen);
        ImGui::EndMenu();
    }
}
