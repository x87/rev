#include "StdInc.h"

#include "COnscreenTimerEntry.h"

// 0x44CB10
void COnscreenTimerEntry::Process() {
    // return plugin::CallMethod<0x44CB10, COnscreenTimerEntry*>(this);

    if (m_nVarId == 0)
        return;

    int deltaTime = int(CTimer::ms_fTimeStep * 0.02f * 1000.0f);
    auto& timer = CTheScripts::ScriptSpace[m_nVarId];
    if (m_nTimerDirection == eTimerDirection::DECREASE) {
        timer -= deltaTime;
        if (timer >= 0) {
            if ((timer / 1000) < m_nClockBeepCountdownSecs && !TheCamera.m_bWideScreenOn) {
                AudioEngine.ReportFrontendAudioEvent(AE_FRONTEND_TIMER_COUNT, 0.0f, 1.0f);
            }
        } else {
            timer = 0;
            m_nVarId = 0;
            m_szDescriptionTextKey[0] = '\0';
            m_bEnabled = false;
        }
    } else {
        timer += deltaTime;
    }
    printf("%d\n", timer);
}

// 0x44CA40
void COnscreenTimerEntry::ProcessForDisplayClock() {
    sprintf(
        m_szDisplayedText,
        "%02d:%02d",
        CTheScripts::ScriptSpace[m_nVarId] / 1000 / 60 % 100,
        CTheScripts::ScriptSpace[m_nVarId] / 1000 % 60
    );
}
