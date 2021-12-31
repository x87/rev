/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#include "StdInc.h"

void CTrainNode::InjectHooks() {
    ReversibleHooks::Install("CTrainNode", "CTrainNode", 0x6F5370, &CTrainNode::Constructor);
    ReversibleHooks::Install("CTrainNode", "SetX", 0x6F5380, &CTrainNode::SetX);
    ReversibleHooks::Install("CTrainNode", "SetY", 0x6F53A0, &CTrainNode::SetY);
    ReversibleHooks::Install("CTrainNode", "SetZ", 0x6F53C0, &CTrainNode::SetZ);
    ReversibleHooks::Install("CTrainNode", "GetX", 0x6F53E0, &CTrainNode::GetX);
    ReversibleHooks::Install("CTrainNode", "GetY", 0x6F5400, &CTrainNode::GetY);
    ReversibleHooks::Install("CTrainNode", "GetZ", 0x6F5420, &CTrainNode::GetZ);
    ReversibleHooks::Install("CTrainNode", "GetPosn", 0x6F5440, &CTrainNode::GetPosn);
    ReversibleHooks::Install("CTrainNode", "SetDistanceFromStart", 0x6F5490, &CTrainNode::SetDistanceFromStart);
    ReversibleHooks::Install("CTrainNode", "GetDistanceFromStart", 0x6F54B0, &CTrainNode::GetDistanceFromStart);
    ReversibleHooks::Install("CTrainNode", "GetLightingFromCollision", 0x6F5F80, &CTrainNode::GetLightingFromCollision);
}

// 0x6F5370
CTrainNode::CTrainNode() {
    // Done by the compiler
}

// 0x6F5370
CTrainNode* CTrainNode::Constructor() {
    this->CTrainNode::CTrainNode();
    return this;
}

// 0x6F5380
void CTrainNode::SetX(float X) {
    m_pos.x = (int16)(X * 8.f);
}

// 0x6F53A0
void CTrainNode::SetY(float Y) {
    m_pos.y = (int16)(Y * 8.f);
}

// 0x6F53C0
void CTrainNode::SetZ(float Z) {
    m_pos.z = (int16)(Z * 8.f);
}

// 0x6F53E0
float CTrainNode::GetX() {
    return (float)m_pos.x / 8.f;
}

// 0x6F5400
float CTrainNode::GetY() {
    return (float)m_pos.y / 8.f;
}

// 0x6F5420
float CTrainNode::GetZ() {
    return (float)m_pos.z / 8.f;
}

// 0x6F5440
CVector CTrainNode::GetPosn() {
    return UncompressLargeVector(m_pos);
}

// 0x6F5490
void CTrainNode::SetDistanceFromStart(float dist) {
    m_nDistanceFromStart = (uint16)(dist * 3.f);
}

// 0x6F54B0
float CTrainNode::GetDistanceFromStart() {
    return (float)m_nDistanceFromStart / 3.f;
}

// 0x6F5F80
tColLighting CTrainNode::GetLightingFromCollision() {
    if (m_bSurfLightingFound)
        return m_nSurfaceLighting;

    CColPoint cp{};
    if (CEntity* he; !CWorld::ProcessVerticalLine(GetPosn() + CVector{ 0.f, 0.f, 1.f }, -1000.f, cp, he, true, false, false, false, false, false, nullptr))
        return { 72 };

    m_nSurfaceLighting = cp.m_nLightingB;
    m_bSurfLightingFound = true;
    return m_nSurfaceLighting;
}
