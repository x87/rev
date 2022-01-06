#include "StdInc.h"

#ifdef EXTRA_DEBUG_FEATURES

#include "CStreamingDebugModule.h"
#include <Streaming.h>

#include "imgui.h"

namespace CStreamingDebugModule {
void ProcessImGUI() {
    using namespace ImGui;

    if (Button("ReInit")) {
        CStreaming::ReInit();
    }

    SameLine(GetColumnWidth());
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

    constexpr auto GetListSize = [](auto begin, auto end) {
        uint32 n{};
        for (auto info = end->GetPrev(); info != begin; info = info->GetPrev())
            n++;
        return n;
    };

    Text("Loaded: %u Ki (%u models)",
        CStreaming::ms_memoryUsed / 1024,
        CStreaming::ms_numModelsLoaded
    );

    {
        const auto mem = CStreaming::GetRequestsMemUsage();
        Text("Requested: %u Sectors (%u Ki) (%u models)", mem, mem * STREAMING_SECTOR_SIZE / 1024, CStreaming::GetNumRequests());
    }

    Text("Loading big model: %i", (int32)CStreaming::ms_bLoadingBigModel);
}

void ProcessRender() {

}
};
#endif
