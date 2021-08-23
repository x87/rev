#pragma once

// TDAT block
class CData {
public:
    char*    data;
    uint32_t size;

public:
    CData();
    ~CData();

    void Unload();
    bool Load(uint32_t length, FILESTREAM file, uint32_t* offset, bool dontRead);
};
