#include "StdInc.h"
#include "CSkidmark.h"

void CSkidmark::RegisterNewPart(CVector posn, CVector2D dir, float length, bool& bloodState) {
    if ((m_type == eSkidMarkType::BLOODY) == bloodState) {
        m_bActive = true;
        if (CTimer::GetTimeInMilliseconds() - m_lastDisappearTimeUpdateMs <= 100) {
            m_vPosn[m_nNumParts] = posn; // Update existing, because of low delta time
        } else {
            if (m_nNumParts >= SKIDMARK_NUM_PARTS - 1) { // The 0th "part" isn't considered as an actual part, so at most we can have this many
                m_lastDisappearTimeUpdateMs = CTimer::GetTimeInMilliseconds();
                m_nState = eSkidmarkState::DISAPPEARING;
                m_fadeBeginMs = CTimer::GetTimeInMilliseconds() + 10'000;
                m_disappearAtMs = CTimer::GetTimeInMilliseconds() + 20'000;
                bloodState = false;
            } else {
                m_nNumParts++;
                m_vPosn[m_nNumParts] = posn;

                const CVector prevPosn = m_vPosn[m_nNumParts - 1];
                CVector2D dirToPrevPart = {
                    posn.y - prevPosn.y, // Swapped intentionally
                    prevPosn.x - posn.x
                };

                dirToPrevPart.Normalise();
                dir.Normalise();

                const float absSqMag = 1.0f + fabs(dirToPrevPart.x * dir.x + dirToPrevPart.y * dir.y);
                m_partDirX[m_nNumParts] = absSqMag * dirToPrevPart.x * length / 2.0f;
                m_partDirY[m_nNumParts] = absSqMag * dirToPrevPart.y * length / 2.0f;

                if (m_nNumParts == 1) {
                    m_partDirX[0] = m_partDirX[1];
                    m_partDirY[0] = m_partDirY[1];
                }

                if (m_nNumParts > 8)
                    bloodState = false;
            }
        }
    } else {
        m_nState = eSkidmarkState::DISAPPEARING;
        m_fadeBeginMs = CTimer::GetTimeInMilliseconds() + 10'000;
        m_disappearAtMs = CTimer::GetTimeInMilliseconds() + 20'000;
    }
}

void CSkidmark::Init(uint32_t id, CVector posn, eSkidMarkType type, bool& bloodState) {
    m_id = id;
    m_vPosn[0] = posn;
    m_partDirY[0] = 0.0f;
    m_partDirX[0] = 0.0f;
    m_bActive = true;
    m_nState = eSkidmarkState::JUST_UPDATED;
    m_lastDisappearTimeUpdateMs = CTimer::GetTimeInMilliseconds() - 1'000;
    m_nNumParts = 0;
    m_type = bloodState ? eSkidMarkType::BLOODY : type;
}

void CSkidmark::Update() {
    const auto timeMS = CTimer::GetTimeInMilliseconds();
    switch (m_nState) {
    case eSkidmarkState::JUST_UPDATED: {
        if (m_bActive)
            break;

        const auto UpdateTime = [this, timeMS](uint32_t low, uint32_t high) {
            m_fadeBeginMs += timeMS + low;
            m_disappearAtMs += timeMS + high;
        };

        m_nState = eSkidmarkState::DISAPPEARING;
        if (m_nNumParts < 4)
            UpdateTime(2500, 5000);
        else if (m_nNumParts >= 9)
            UpdateTime(10'000, 20'000);
        else // 4 - 8 parts
            UpdateTime(5'000, 10'000); 
        break;
    }
    case eSkidmarkState::DISAPPEARING: {
        if (timeMS > m_disappearAtMs)
            m_nState = eSkidmarkState::INACTIVE;
        break;
    }
    }
    m_bActive = false;
}

CRGBA CSkidmark::GetColor() const {
    const auto GetBaseColor = [this]() -> CRGBA {
        switch (m_type) {
        case eSkidMarkType::DEFALT:
            return { 0, 0, 0, 255 };

        case eSkidMarkType::SANDY:
            return { 45, 31, 4, 255 };

        case eSkidMarkType::MUDDY:
            return { 69, 69, 61, 255 };

        case eSkidMarkType::BLOODY:
            return { 32, 0, 0, 255 };

        default: {
            assert(0); // NOTSA
            return {};
        }
        }
    };

    const auto GetAlpha = [this]() -> uint8 {
        if (m_nState == eSkidmarkState::JUST_UPDATED || CTimer::GetTimeInMilliseconds() < m_fadeBeginMs)
            return (uint8)255;
        return (255 * (m_disappearAtMs - CTimer::GetTimeInMilliseconds())) / (m_disappearAtMs - m_fadeBeginMs);
    };

    // TODO: This balance param stuff is probably inlined
    const float balance = ((1.0f - CCustomBuildingDNPipeline::m_fDNBalanceParam) * 0.6f + 0.4f);
    CRGBA balancedColor = GetBaseColor() * balance;
    balancedColor.a = GetAlpha();

    return balancedColor;
}

void CSkidmark::Render() const {
    if (m_nState == eSkidmarkState::INACTIVE)
        return;

    if (!m_nNumParts)
        return;

    const auto GetColorForPart = [&, baseColor = GetColor()](unsigned part) {
        const auto GetAlpha = [&]() {         
            if (part == 0)
                return 0; // First "part" has always 0 alpha.
            if (m_nNumParts == part && m_nState == eSkidmarkState::JUST_UPDATED)
                return 0; // Last part
            return baseColor.a / 2;
        };
        RwRGBA partColor = baseColor.ToRwRGBA();
        partColor.alpha = GetAlpha();
        return partColor;
    };

    for (unsigned part = 0; part <= m_nNumParts; part++) {
        const CVector posn = m_vPosn[part];
        const CVector dir = { m_partDirX[part], m_partDirY[part], 0.0f };
        const CVector pos[] = {
            posn + dir + CVector{ {}, {}, 0.1f },
            posn - dir + CVector{ {}, {}, 0.1f },
        };
        const     float coordU    = (float)(part) * 5.01f;
        constexpr float coordsV[] = { 0.0f, 1.0f };
        const     RwRGBA color    = GetColorForPart(part);
        for (unsigned vert = 0; vert < 2; vert++) {
            RxObjSpace3DVertex* vertex = &aTempBufferVertices[2 * part + vert];

            RxObjSpace3DVertexSetPreLitColor(vertex, &color);
            RxObjSpace3DVertexSetPos(vertex, &pos[vert]);
            RxObjSpace3DVertexSetU(vertex, coordsV[vert]);
            RxObjSpace3DVertexSetV(vertex, coordU);
        }
    }

    LittleTest();
    if (RwIm3DTransform(aTempBufferVertices, 2 * m_nNumParts + 2, nullptr, rwIM3D_VERTEXUV)) {
        RwIm3DRenderIndexedPrimitive(rwPRIMTYPETRILIST, CSkidmarks::m_aIndices, 6 * m_nNumParts);
        RwIm3DEnd();
    }
}
