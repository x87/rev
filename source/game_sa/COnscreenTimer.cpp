#include "StdInc.h"

#include "COnscreenTimer.h"
#include "COnscreenTimerEntry.h"

void COnscreenTimer::InjectHooks() {
    ReversibleHooks::Install("COnscreenTimer", "AddClock", 0x44CD50, &COnscreenTimer::AddClock);
    ReversibleHooks::Install("COnscreenTimer", "ClearClock", 0x44CE60, &COnscreenTimer::ClearClock);
    ReversibleHooks::Install("COnscreenTimer", "SetClockBeepCountdownSecs", 0x44CEE0, &COnscreenTimer::SetClockBeepCountdownSecs);
    ReversibleHooks::Install("COnscreenTimer", "AddCounter", 0x44CDA0, &COnscreenTimer::AddCounter);
    ReversibleHooks::Install("COnscreenTimer", "ClearCounter", 0x44CE80, &COnscreenTimer::ClearCounter);
    ReversibleHooks::Install("COnscreenTimer", "SetCounterFlashWhenFirstDisplayed", 0x44CEB0, &COnscreenTimer::SetCounterFlashWhenFirstDisplayed);
}

// 0x44CBC0
void COnscreenTimer::Init() {
    for (auto& counter : m_aCounters) {
        counter.Init();
    }

    m_bPaused = false;
    m_Clock.m_nVarId = 0;

    m_Clock.m_szDescriptionTextKey[0] = '\0';
    m_Clock.m_szDescriptionTextKey[4] = '\0';
    m_Clock.m_szDescriptionTextKey[8] = '\0';

    m_Clock.m_bEnabled = false;
    m_Clock.m_nTimerDirection = eTimerDirection::INCREASE; // 1;
    m_Clock.m_nClockBeepCountdownSecs = 12;
}

// 0x44CD50
void COnscreenTimer::AddClock(unsigned int varId, char* gxt, eTimerDirection nTimerDirection) {
    if (m_Clock.m_nVarId != 0)
        return;

    m_Clock.m_nVarId = varId;
    m_Clock.m_nTimerDirection = nTimerDirection;
    if (gxt)
        strncpy(m_Clock.m_szDescriptionTextKey, gxt, 10);
    else
        m_Clock.m_szDescriptionTextKey[0] = '\0';
}

// 0x44CE60
void COnscreenTimer::ClearClock(unsigned int varId) {
    if (m_Clock.m_nVarId == varId) {
        m_Clock.m_nVarId = 0;
        m_Clock.m_szDescriptionTextKey[m_Clock.m_nVarId] = '\0';
        m_Clock.m_bEnabled = false;
        m_Clock.m_nTimerDirection = eTimerDirection::INCREASE; // 1;
    }
}

// 0x44CEE0
void COnscreenTimer::SetClockBeepCountdownSecs(unsigned int varId, unsigned int time) {
    for (auto& counter : m_aCounters) {
        if (m_Clock.m_nVarId == varId) {
            m_Clock.m_nClockBeepCountdownSecs = time;
        }
    }
}

// 0x44CDA0
void COnscreenTimer::AddCounter(int varId, eOnscreenCounter type, char* gxt, unsigned short counterIndex) {
    COnscreenCounterEntry* counter = &m_aCounters[counterIndex];
    if (counter->m_nVarId != 0)
        return;

    counter->m_nVarId = varId;
    if (gxt)
        strncpy(counter->m_szDescriptionTextKey, gxt, 10);
    else
        counter->m_szDescriptionTextKey[0] = '\0';

    counter->m_nMaxVarValue = 0;
    counter->m_nType = type;
    counter->m_bFlashWhenFirstDisplayed = true;
    counter->m_nColourId = HUD_COLOUR_LIGHT_BLUE;
}

// unused
// 0x44CE00
void COnscreenTimer::AddCounterCounter(unsigned int varId, unsigned int maxValue, char* gxt, unsigned short lineId) {
    COnscreenCounterEntry* counter = &m_aCounters[lineId];
    if (counter->m_nVarId != 0)
        return;

    counter->m_nVarId = varId;
    counter->m_nMaxVarValue = maxValue;
    if (gxt)
        strncpy(counter->m_szDescriptionTextKey, gxt, 10);
    else
        counter->m_szDescriptionTextKey[0] = '\0';

    counter->m_nType = eOnscreenCounter::DOUBLE;
    counter->m_bFlashWhenFirstDisplayed = true;
}

// 0x44CE80
void COnscreenTimer::ClearCounter(unsigned int varId) {
    for (auto& counter : m_aCounters) {
        if (counter.m_nVarId == varId) {
            counter.Clear();
        }
    }
}

// unused
// 0x44CF10
void COnscreenTimer::SetCounterColourID(unsigned int varId, eHudColours colorId) {
    for (auto& counter : m_aCounters) {
        if (counter.m_nVarId == varId) {
            counter.SetColourID(colorId);
        }
    }
}

// 0x44CEB0
void COnscreenTimer::SetCounterFlashWhenFirstDisplayed(unsigned int varId, bool bFlashWhenFirstDisplayed) {
    for (auto& counter : m_aCounters) {
        if (counter.m_nVarId == varId) {
            counter.m_bFlashWhenFirstDisplayed = bFlashWhenFirstDisplayed;
        }
    }
}

// 0x44CD30
void COnscreenTimer::Process() {
    if (CReplay::Mode == REPLAY_MODE_1 || m_bPaused)
        return;

    m_Clock.Process();
}

// 0x44CC20
void COnscreenTimer::ProcessForDisplay() {
    // return plugin::CallMethod<0x44CC20, COnscreenTimer*>(this);

    if (!CHud::m_Wants_To_Draw_Hud)
        return;

    m_bDisplay = false;
    m_Clock.m_bEnabled = false;

    if (m_Clock.m_nVarId != 0) {
        m_Clock.ProcessForDisplayClock();
        m_Clock.m_bEnabled = true;
        m_bDisplay = true;
    }

    for (auto& counter : m_aCounters) {
        counter.m_bEnabled = false;
        if (counter.m_nVarId != 0) {
            counter.ProcessForDisplayCounter(counter.m_nType);
            counter.m_bEnabled = true;
            m_bDisplay = true;
        }
    }
}
