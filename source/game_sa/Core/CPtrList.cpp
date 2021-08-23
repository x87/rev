#include "StdInc.h"

void CPtrList::InjectHooks() {
    ReversibleHooks::Install("CPtrList", "CountElements", 0x5521B0, &CPtrList::CountElements);
    ReversibleHooks::Install("CPtrList", "IsMemberOfList", 0x5521D0, &CPtrList::IsMemberOfList);
}

CPtrList::CPtrList() {
    pNode = nullptr;
}

// get elements count
// 0x5521B0
uint32_t CPtrList::CountElements() {
    uint32_t counter;
    CPtrNode* currNode = GetNode();
    for (counter = 0; currNode; ++counter) {
        currNode = currNode->pNext;
    }
    return counter;
}

// check if data is a member of list
// 0x5521D0
bool CPtrList::IsMemberOfList(void* data) {
    CPtrNode* currNode = GetNode();
    while (currNode) {
        if (currNode->pItem == data) {
            return true;
        }
        currNode = currNode->pNext;
    }
    return false;
}
