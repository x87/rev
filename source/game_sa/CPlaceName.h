#pragma once

class CZone;

class CPlaceName {
public:
    CZone*   m_pZone;
    uint16_t m_nAdditionalTimer;
    char     _pad[2];

public:
    static void InjectHooks();

    void Init();
    void Remove();
    inline void Update() const;
    const char* GetForMap(float x, float y);
    void Process();
};

VALIDATE_SIZE(CPlaceName, 0x8);
