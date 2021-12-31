/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Vector.h"

class CTrainNode {
public:
    CompressedVector m_pos;
    uint16           m_nDistanceFromStart{};
    tColLighting     m_nSurfaceLighting{};
    bool             m_bSurfLightingFound{};

    CTrainNode();
    CTrainNode* Constructor();

    static void InjectHooks();

    void    SetX(float X);
    void    SetY(float Y);
    void    SetZ(float Z);
    float   GetX();
    float   GetY();
    float   GetZ();
    CVector GetPosn();
    void    SetDistanceFromStart(float dist);
    float   GetDistanceFromStart();
    tColLighting GetLightingFromCollision();
};

VALIDATE_SIZE(CTrainNode, 0xA);
