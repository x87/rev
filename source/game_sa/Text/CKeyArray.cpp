#include "StdInc.h"

#include "CKeyArray.h"
#include "GxtChar.h"

CKeyArray::CKeyArray() {
    data = nullptr;
    size = 0;
}

CKeyArray::~CKeyArray() {
    Unload();
}

// 0x69F510
void CKeyArray::Unload() {
    delete[] data;
    data = nullptr;
    size = 0;
}

#define USE_ORIGINAL_CODE

// dontRead always 0
// 0x69F490
bool CKeyArray::Load(uint32_t length, FILESTREAM file, uint32_t* offset, bool dontRead) {
#ifdef USE_ORIGINAL_CODE
    uint32_t temp = 0;

    size = length / sizeof(CKeyEntry);
    data = new CKeyEntry[size];

    if (length) {
        if (length != CFileMgr::Read(file, data, length)) {
            return false;
        }

        *offset += length;
    }

    return true;
#else
    // taken from re3
    size = length / sizeof(CKeyEntry);
    data = new CKeyEntry[size];

    CFileMgr::Read(file, data, length);
    *offset += length;
#endif
}

// 0x69F540
void CKeyArray::Update(char* offset) {
    for (uint32_t i = 0; i < size; ++i) {
        data[i].string = (GxtChar*)((uchar*)offset + (uint32_t)(data[i].string));
    }
}

// 0x69F570
CKeyEntry* CKeyArray::BinarySearch(uint32_t hash, CKeyEntry* entries, int16_t firstIndex, int16_t lastIndex) {
    if (firstIndex > lastIndex)
        return nullptr;

    while (true) {
        uint16_t middle = (firstIndex + lastIndex) >> 1;
        auto entryHash = entries[middle].hash;

        if (hash == entryHash)
            return &entries[middle];

        if (hash > entryHash)
            firstIndex = middle + 1;
        else
            lastIndex = middle - 1;

        if (firstIndex > lastIndex)
            return nullptr;
    }
}

// 0x6A0000
char* CKeyArray::Search(const char* key, bool* found) {
    uint32_t hash = CKeyGen::GetUppercaseKey(key);
    CKeyEntry* entry = BinarySearch(hash, data, 0, size - 1);
    if (entry) {
        *found = true;
        return entry->string;
    } else {
        *found = false;
        return nullptr;
    }
}
