#pragma once

struct CKeyEntry {
    char*    string; // relative to TKEY block; need to call update after reading
    uint32_t hash;   // name hash
};

// TKEY block
class CKeyArray {
public:
    CKeyEntry* data;
    uint32_t   size;

public:
    CKeyArray();
    ~CKeyArray();

    void Unload();
    bool Load(uint32_t length, FILESTREAM file, uint32_t* offset, bool dontRead);

    void /* inline */ Update(char* offset);

    CKeyEntry* BinarySearch(uint32_t key, CKeyEntry* entries, int16_t firstIndex, int16_t lastIndex);
    char* Search(const char* key, bool* found);
};
