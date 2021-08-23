#pragma once

#include "CText.h"

struct tTextOffset {
    char     szMissionName[8];
    uint32_t offset;
};

// TABL block
class CMissionTextOffsets {
public:
    static const ushort MAX_MISSION_TEXTS = 200;

    tTextOffset data[MAX_MISSION_TEXTS];
    uint32_t    size;

public:
    CMissionTextOffsets();

    bool Load(uint32_t length, FILE* file, uint32_t* offset, bool dontRead);
};
