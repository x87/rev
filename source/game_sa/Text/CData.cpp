#include "StdInc.h"

#include "CData.h"
#include "GxtChar.h"

CData::CData() {
    data = nullptr;
    size = 0;
}

CData::~CData() {
    Unload();
}

// 0x69F640
void CData::Unload() {
    delete[] data;
    data = nullptr;
    size = 0;
}

#define USE_ORIGINAL_CODE

// dontRead always 0
// 0x69F5D0
bool CData::Load(uint32_t length, FILESTREAM file, uint32_t* offset, bool dontRead) {
    size = length / sizeof(GxtChar);
    data = new GxtChar[size];

#ifdef USE_ORIGINAL_CODE
    uint32_t temp = 0;

    if (!length)
        return true;

    for (uint32_t i = 0; i < size; ++i) {
        if (!dontRead)
            if (sizeof(GxtChar) != CFileMgr::Read(file, &temp, sizeof(GxtChar)))
                return false;

        data[i] = (GxtChar)temp;
        ++*offset;
    }

    return true;
#else
    CFileMgr::Read(file, data, length);
    *offset += length;
#endif
}
