#include "StdInc.h"

#include "CMissionTextOffsets.h"

CMissionTextOffsets::CMissionTextOffsets() {
    size = 0;
}

// 0x69F670
bool CMissionTextOffsets::Load(uint32_t length, FILESTREAM file, uint32_t* offset, bool dontRead) {
    tTextOffset* pTextOffset = data;
    size = 0;

    while (true) {
        if (sizeof(tTextOffset) != CFileMgr::Read(file, pTextOffset, sizeof(tTextOffset))) {
            return false;
        }
        *offset += sizeof(tTextOffset);

        ++pTextOffset;
        ++size;

        if (uint32_t(pTextOffset - data) >= length / sizeof(tTextOffset)) {
            break;
        }
    }

    return true;

#ifdef USE_ORIGINAL_CODE
    // todo: taken from re3 and not tested!
    size_t entryCount = length / sizeof(tTextOffset);
    for (size_t mi = 0; mi < entryCount; mi++) {
        for (unsigned int i = 0; i < sizeof(data[mi].szMissionName); i++) {
            CFileMgr::Read(file, &data[i].szMissionName[i], 1);
            (*offset)++;
        }
        char* _buf = (char*)&data[mi].offset;
        for (unsigned int i = 0; i < sizeof(data[mi].offset); i++) {
            CFileMgr::Read(file, &_buf[i], 1);
            (*offset)++;
        }
    }
    size = (unsigned short)entryCount;
#else
    size = (uint16_t) (length / sizeof(tTextOffset));
    CFileMgr::Read(file, (char*)data, sizeof(tTextOffset) * size);
    *offset += sizeof(tTextOffset) * size;
#endif
}
