#pragma once

class CVehicle;

class CCurrentVehicle {
public:
    CVehicle* vehicle;

public:
    static void InjectHooks();

    CCurrentVehicle();

    void Init();
    void Display() const;
    void Process();
};
