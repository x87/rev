/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "CHudColours.h"
#include "eOnscreenCounter.h"

class COnscreenCounterEntry {
public:
    unsigned int     m_nVarId;
    unsigned int     m_nMaxVarValue;
    char             m_szDescriptionTextKey[10];
    eOnscreenCounter m_nType;
    char             m_szDisplayedText[42]; // possibly 2b padding?
    bool             m_bEnabled;
    bool             m_bFlashWhenFirstDisplayed;
    eHudColours      m_nColourId;

public:
    static void InjectHooks();

    void Init();
    void Clear();

    void ProcessForDisplayCounter(eOnscreenCounter type);
    void SetColourID(eHudColours colourId);
};

VALIDATE_SIZE(COnscreenCounterEntry, 0x44);
