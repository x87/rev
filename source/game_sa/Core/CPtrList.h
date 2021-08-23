/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CPtrNode.h"

class CPtrList {
public:
    CPtrNode* pNode;

public:
    static void InjectHooks();

    CPtrList();

    uint32_t CountElements();
    bool IsMemberOfList(void* data);

    // NOTSA
    CPtrNode* GetNode() const {
        return pNode;
    }
};

VALIDATE_SIZE(CPtrList, 0x4);
