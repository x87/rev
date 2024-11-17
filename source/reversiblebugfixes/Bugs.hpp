#pragma once

#include "./ReversibleBugFix.hpp"

namespace notsa::bugfixes {
    inline const ReversibleBugFix PS2CoronaRendering{
        "PS2 Corona Rendering",
        "Fix corona rendering, so they're like on PS2",
        "SilentPatch Contributors"
    };
    inline const ReversibleBugFix AnimBlendSequence_SetName_SetBoneTagFlag{
        "BoneTag Name Flag",
        "Correctly set BoneTag flag in CAnimBlendSequence::SetName",
        "Pirulax"
    };
    inline const ReversibleBugFix AESound_UpdatePlayTime_DivisionByZero{
        "UpdatePlayTime Division-By-Zero",
        "Avoid Division-by-zero in CAESound::UpdatePlayTime",
        "Pirulax"
    };
};
