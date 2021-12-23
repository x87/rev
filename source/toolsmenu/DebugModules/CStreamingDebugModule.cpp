#include "StdInc.h"

#include "ResourceStreamer.h"

#ifdef EXTRA_DEBUG_FEATURES

#include "CStreamingDebugModule.h"
#include <Streaming.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <imgui_internal.h>

namespace CStreamingDebugModule {
void ProcessImGUI() {
    using namespace ImGui;

    if (Button("ReInit")) {
        CStreaming::ReInit();
    }

    SameLine();

    if (Button("Rnd. teleport")) {
        const CVector2D pos{
            CGeneral::GetRandomNumberInRange(-3072.0f, 3072.0f),
            CGeneral::GetRandomNumberInRange(-3072.0f, 3072.0f),
        };
        FindPlayerPed()->Teleport({
            pos.x, 
            pos.y,
            CWorld::FindGroundZForCoord(pos.x, pos.y)
        }, true);
    }

    //SameLine();

    Separator();

    Text("Memory: %u Ki", CStreaming::ms_memoryUsed / 1024);
    Text("Buffer size: %u Sectors (%u Ki)", CStreaming::ms_streamingBufferSize, CStreaming::ms_streamingBufferSize * ResourceStreamer::STREAMING_SECTOR_SIZE / 1024);

    {
        constexpr auto GetListSize = [](auto begin, auto end) {
            uint32 n{};
            for (auto it = begin->GetNext(); it != end; it = it->GetNext())
                n++;
            return n;
        };

        Columns(2);

        const auto w = GetWindowWidth();
        static bool widthSet = false;
        Columns(2);
        if (!widthSet) {
            widthSet = true;
            SetColumnWidth(0, w * 0.8f);
            SetColumnWidth(1, w * 0.2f);
        }

        //TextUnformatted("What");
        //NextColumn();

        //TextUnformatted("Count");
        //NextColumn();
        Separator();

        /*const struct {
            const char* what;
            size_t count;
        }*/
        const std::pair<const char*, size_t> items[] = {
            {"#Requests: %u", CStreaming::ms_numModelsRequested},
            {"#Priority requests: %u", CStreaming::ms_numPriorityRequests},
            {"#Peds loaded: %u", CStreaming::ms_numPedsLoaded},
            {"#Veh loaded: %u", CStreaming::ms_vehiclesLoaded.CountMembers()},
            {"#Total loaded: %u", GetListSize(CStreaming::ms_startLoadedList, CStreaming::ms_pEndLoadedList)},
        };

        for (auto [what, count] : items) {
            Text(what, count);
        }

        //EndColumns();
    }

    Separator();

    {
        constexpr auto GetChannelStatusStr = [](auto ch) {
            switch (CStreaming::ms_channel[ch].LoadStatus) {
            case eChannelState::IDLE:
                return "IDLE";
            case eChannelState::ERR:
                return "ERR";
            case eChannelState::STARTED:
                return "STARTED";
            case eChannelState::READING:
                return "READING";
            default:
                return "Unknown";
            }
        };

        Text("Ch #0 status: %s", GetChannelStatusStr(0));
        Text("Ch #1 status: %s", GetChannelStatusStr(1));
    }

    Separator();

    {
        const std::pair<const char*, int32> items[] = {
            {"ms_bLoadingBigModel: %i", (int32)CStreaming::ms_bLoadingBigModel},
            {"m_bLoadingAllRequestedModels: %i", (int32)CStreaming::m_bLoadingAllRequestedModels},
            {"m_bModelStreamNotLoaded: %i", (int32)CStreaming::m_bModelStreamNotLoaded},
            {"ms_bLoadingScene: %i", (int32)CStreaming::ms_bLoadingScene},
        };

        for (auto [what, state] : items) {
            Text(what, state);
        }
    }

    Separator();
}

void ProcessRender() {

}
};
#endif
