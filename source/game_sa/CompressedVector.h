/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

class CompressedVector {
public:
    int16 x;
    int16 y;
    int16 z;

    // Multipliers depend on vector type, so they can't simply be added
    // maybe create a types of compressed vectors and then add these
    //void SetX(float x) noexcept { this->x = (int16)(x * 8.f); }
    //void SetY(float y) noexcept { this->y = (int16)(y * 8.f); }
    //void SetZ(float z) noexcept { this->z = (int16)(z * 8.f); }

    //float GetX() const noexcept { return (float)x / 8.f; }
    //float GetY() const noexcept { return (float)y / 8.f; }
    //float GetZ() const noexcept { return (float)z / 8.f; }
};

VALIDATE_SIZE(CompressedVector, 0x6);

CVector UncompressVector(const CompressedVector& compressedVec);
CompressedVector CompressVector(const CVector& vec);
float UncompressUnitFloat(int16 val);
CVector UncompressUnitVector(const CompressedVector& compressedVec);
CompressedVector CompressUnitVector(CVector const& vec);
CVector UncompressLargeVector(const CompressedVector& compressedVec);
CompressedVector CompressLargeVector(CVector const& vec);
