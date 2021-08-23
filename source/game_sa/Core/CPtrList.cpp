#include "StdInc.h"

void CPtrList::InjectHooks() {
    ReversibleHooks::Install("CPtrList", "CountElements", 0x5521B0, &CPtrList::CountElements);
    ReversibleHooks::Install("CPtrList", "IsMemberOfList", 0x5521D0, &CPtrList::IsMemberOfList);
}

CPtrList::CPtrList() {
    pNode = nullptr;
}

CPtrNode* CPtrList::GetNode() {
    return pNode;
}

// get elements count
// 0x5521B0
unsigned int CPtrList::CountElements() {
    unsigned int counter;
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
