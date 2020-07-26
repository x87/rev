#include "StdInc.h"

std::int32_t CPedGeometryAnalyser::ComputeEntityHitSide(CVector const& point, CEntity const& entity)
{
    return plugin::CallAndReturn<std::int32_t, 0x5F3AC0, CVector const&, CEntity const&>(point, entity);
}

std::int32_t CPedGeometryAnalyser::ComputeEntityHitSide(CPed const& ped, CEntity const& entity)
{
    return plugin::CallAndReturn<std::int32_t, 0x5F3BC0, CPed const&, CEntity const&>(ped, entity);
}

void CPedGeometryAnalyser::ComputeEntityBoundingBoxPlanes(float zPos, CEntity& entity, CVector* planes, float* planes_D)
{
    plugin::Call<0x5F3660, float, CEntity&, CVector*, float*>(zPos, entity, planes, planes_D);
}

void CPedGeometryAnalyser::ComputeEntityBoundingBoxPlanesUncached(float zPos, CVector const* corners, CVector* planes, float* planes_D)
{
#ifdef USE_DEFAULT_FUNCTIONS
    plugin::Call<0x5F1670, float, CVector const*, CVector*, float*>(zPos, corners, planes, planesDot);
#else
    CVector const* corner2 = &corners[3];
    for (std::int32_t i = 0; i < 4; i++) {
        CVector const& corner = corners[i];
        CVector& plane = planes[i];
        CVector direction = corner - *corner2;
        direction.Normalise();
        plane.x = direction.y;
        plane.y = -direction.x;
        plane.z = 0.0f;
        // point-normal plane equation:
        // ax + by + cz + d = 0
        // d = - n . P
        planes_D[i] = -DotProduct(plane, *corner2);
        corner2 = &corner;
    }
#endif
}
