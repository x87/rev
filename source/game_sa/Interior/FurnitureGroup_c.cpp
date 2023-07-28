#include "StdInc.h"
#include "FurnitureGroup_c.h"

auto& g_currSubGroupId = StaticRef<uint16, 0xBAB37C>();
auto& g_subGroupStore  = StaticRef<std::array<FurnitureSubGroup_c, 128>, 0xBAD3F8>();

void FurnitureGroup_c::InjectHooks() {
    RH_ScopedClass(FurnitureGroup_c);
    RH_ScopedCategory("Interior");

    RH_ScopedInstall(Init, 0x5910A0, { .reversed = false });
    RH_ScopedInstall(Exit, 0x5910B0, { .reversed = false });
    RH_ScopedInstall(AddSubGroup, 0x5910E0, { .reversed = false });
    RH_ScopedInstall(GetFurniture, 0x591130, { .reversed = false });
    RH_ScopedInstall(GetRandomId, 0x591170, { .reversed = false });
    RH_ScopedInstall(AddFurniture, 0x5C0230, { .reversed = false });
}

// 0x5910A0
int32 FurnitureGroup_c::Init() {
    return 1;
}

// 0x5910B0
int32 FurnitureGroup_c::Exit() {
    for (auto it = m_subGroupsList.GetHead(); it; it = m_subGroupsList.GetNext(it)) {
        it->m_furnitureList.RemoveAll();
    }
    m_subGroupsList.RemoveAll();
}

// 0x5910E0
bool FurnitureGroup_c::AddSubGroup(int32 subGroupId, int32 minW, int32 minD, int32 maxW, int32 maxD, bool onWindow, bool isTall, bool isStealable) {
    if (g_currSubGroupId >= std::size(g_subGroupStore)) {
        return false;
    }
    auto& sg = g_subGroupStore[g_currSubGroupId++] = FurnitureSubGroup_c{
        subGroupId,
        onWindow,
        isTall,
        isStealable
    };
    m_subGroupsList.AddItem(&sg);
    return true;
}

// 0x591130
Furniture_c* FurnitureGroup_c::GetFurniture(int32 subGroupId, int16 a1, uint8 a2) {
    m_subGroupsList.ForEach([](FurnitureSubGroup_c& gr) {

    });
}

// 0x591170
int32 FurnitureGroup_c::GetRandomId(int32 subgroupId, uint8 a2) {
    return plugin::CallMethodAndReturn<int32, 0x591170, FurnitureGroup_c*, int32, uint8>(this, subgroupId, a2);
}

// 0x5C0230
bool FurnitureGroup_c::AddFurniture(int32 subgroupId, uint16 modelId, int16 id, uint8 wealthMin, uint8 wealthMax, uint8 maxAng) {
    return plugin::CallMethodAndReturn<int8, 0x5C0230, FurnitureGroup_c*, int32, uint16, int16, uint8, uint8, uint8>(this, subgroupId, modelId, id, wealthMin, wealthMax, maxAng);
}
