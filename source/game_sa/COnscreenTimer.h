/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "COnscreenTimerEntry.h"
#include "COnscreenCounterEntry.h"

#include "CHudColours.h"
#include "eOnscreenCounter.h"

class COnscreenTimer {
public:
    COnscreenTimerEntry m_Clock;
    COnscreenCounterEntry m_aCounters[4];
    bool m_bDisplay;
    bool m_bPaused;

public:
    static void InjectHooks();

    void Init();

    void AddClock(unsigned int varId, char* gxt, eTimerDirection nTimerDirection);
    void ClearClock(unsigned int varId);
    void SetClockBeepCountdownSecs(unsigned int varId, unsigned int time);

    void AddCounter(int varId, eOnscreenCounter type, char* gxt, unsigned short counterIndex);
    void AddCounterCounter(unsigned int varId, unsigned int maxValue, char* gxt, unsigned short lineId);
    void ClearCounter(unsigned int varId);
    void SetCounterColourID(unsigned int varId, eHudColours colorId);
    void SetCounterFlashWhenFirstDisplayed(unsigned int varId, bool bFlashWhenFirstDisplayed);

    void Process();
    void ProcessForDisplay();
};

VALIDATE_SIZE(COnscreenTimer, 0x154);
