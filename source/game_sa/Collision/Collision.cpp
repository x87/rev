/*
    Plugin-SDK (Grand Theft Auto San Andreas) source file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "StdInc.h"

CLinkList<CCollisionData*>& CCollision::ms_colModelCache = *(CLinkList<CCollisionData*>*)0x96592C;

void CalculateColPointInsideBox(CBox const& box, CVector const& point, CColPoint& colPoint); // Forward declaration needed for `InjectHooks`

void CCollision::InjectHooks()
{
    using namespace ReversibleHooks;

    Install("CCollision", "Update", 0x411E20, &CCollision::Update);
    Install("CCollision", "SortOutCollisionAfterLoad", 0x411E30, &CCollision::SortOutCollisionAfterLoad);
    Install("CCollision", "TestSphereSphere", 0x411E70, &CCollision::TestSphereSphere);
    Install("CCollision", "CalculateColPointInsideBox", 0x411EC0, &CalculateColPointInsideBox);
    Install("CCollision", "TestSphereBox", 0x4120C0, &CCollision::TestSphereBox);
    //Install("CCollision", "ProcessSphereBox", 0x412130, &CCollision::ProcessSphereBox);
    //Install("CCollision", "PointInTriangle", 0x412700, &CCollision::PointInTriangle);
    //Install("CCollision", "DistToLineSqr", 0x412850, &CCollision::DistToLineSqr);
    Install("CCollision", "DistToMathematicalLine", 0x412970, &CCollision::DistToMathematicalLine);
    //Install("CCollision", "DistToMathematicalLine2D", 0x412A30, &CCollision::DistToMathematicalLine2D);
    //Install("CCollision", "DistAlongLine2D", 0x412A80, &CCollision::DistAlongLine2D);
    //Install("CCollision", "ProcessLineSphere", 0x412AA0, &CCollision::ProcessLineSphere);
    //Install("CCollision", "TestLineBox_DW", 0x412C70, &CCollision::TestLineBox_DW);
    //Install("CCollision", "TestLineBox", 0x413070, &CCollision::TestLineBox);
    //Install("CCollision", "TestVerticalLineBox", 0x413080, &CCollision::TestVerticalLineBox);
    //Install("CCollision", "ProcessLineBox", 0x413100, &CCollision::ProcessLineBox);
    //Install("CCollision", "Test2DLineAgainst2DLine", 0x4138D0, &CCollision::Test2DLineAgainst2DLine);
    //Install("CCollision", "colPoint1", 0x413960, &ProcessDiscCollision);
    //Install("CCollision", "TestLineTriangle", 0x413AC0, &CCollision::TestLineTriangle);
    //Install("CCollision", "ProcessLineTriangle", 0x4140F0, &CCollision::ProcessLineTriangle);
    //Install("CCollision", "ProcessVerticalLineTriangle", 0x4147E0, &CCollision::ProcessVerticalLineTriangle);
    //Install("CCollision", "IsStoredPolyStillValidVerticalLine", 0x414D70, &CCollision::IsStoredPolyStillValidVerticalLine);
    //Install("CCollision", "GetBoundingBoxFromTwoSpheres", 0x415230, &CCollision::GetBoundingBoxFromTwoSpheres);
    //Install("CCollision", "IsThisVehicleSittingOnMe", 0x4152C0, &CCollision::IsThisVehicleSittingOnMe);
    //Install("CCollision", "CheckCameraCollisionPeds", 0x415320, &CCollision::CheckCameraCollisionPeds);
    //Install("CCollision", "CheckPeds", 0x4154A0, &CCollision::CheckPeds);
    //Install("CCollision", "ResetMadeInvisibleObjects", 0x415540, &ResetMadeInvisibleObjects);
    //Install("CCollision", "SphereCastVsBBox", 0x415590, &CCollision::SphereCastVsBBox);
    //Install("CCollision", "RayPolyPOP", 0x415620, &CCollision::RayPolyPOP);
    //Install("CCollision", "GetPrincipleAxis", 0x4156D0, &CCollision::GetPrincipleAxis);
    //Install("CCollision", "PointInPoly", 0x415730, &CCollision::PointInPoly);
    //Install("CCollision", "Closest3", 0x415950, &CCollision::Closest3);
    //Install("CCollision", "ClosestSquaredDistanceBetweenFiniteLines", 0x415A40, &ClosestSquaredDistanceBetweenFiniteLines);
    //Install("CCollision", "SphereCastVersusVsPoly", 0x415CF0, &CCollision::SphereCastVersusVsPoly);
    //Install("CCollision", "Init", 0x416260, &CCollision::Init);
    //Install("CCollision", "Shutdown", 0x4162E0, &CCollision::Shutdown);
    Install("CCollision", "CalculateTrianglePlanes_colData", 0x416330, static_cast<void(*)(CCollisionData*)>(&CCollision::CalculateTrianglePlanes));
    Install("CCollision", "RemoveTrianglePlanes_colData", 0x416400, static_cast<void(*)(CCollisionData*)>(&CCollision::RemoveTrianglePlanes));
    //Install("CCollision", "ProcessSphereSphere", 0x416450, &CCollision::ProcessSphereSphere);
    //Install("CCollision", "TestSphereTriangle", 0x4165B0, &CCollision::TestSphereTriangle);
    //Install("CCollision", "ProcessSphereTriangle", 0x416BA0, &CCollision::ProcessSphereTriangle);
    //Install("CCollision", "TestLineSphere", 0x417470, &CCollision::TestLineSphere);
    //Install("CCollision", "DistToLine", 0x417610, &CCollision::DistToLine);
    //Install("CCollision", "TestLineOfSight", 0x417730, &CCollision::TestLineOfSight);
    //Install("CCollision", "ProcessLineOfSight", 0x417950, &CCollision::ProcessLineOfSight);
    //Install("CCollision", "ProcessVerticalLine", 0x417BF0, &CCollision::ProcessVerticalLine);
    //Install("CCollision", "SphereCastVsSphere", 0x417F20, &CCollision::SphereCastVsSphere);
    //Install("CCollision", "ClosestPointOnLine", 0x417FD0, &CCollision::ClosestPointOnLine);
    //Install("CCollision", "ClosestPointsOnPoly", 0x418100, &CCollision::ClosestPointsOnPoly);
    //Install("CCollision", "ClosestPointOnPoly", 0x418150, &CCollision::ClosestPointOnPoly);
    //Install("CCollision", "SphereCastVsCaches", 0x4181B0, &CCollision::SphereCastVsCaches);
    //Install("CCollision", "CalculateTrianglePlanes_colModel", 0x416330, static_cast<void(*)(CColModel*)>(&CCollision::CalculateTrianglePlanes));
    //Install("CCollision", "RemoveTrianglePlanes_colModel", 0x416400, static_cast<void(*)(CColModel*)>(&CCollision::RemoveTrianglePlanes));
    //Install("CCollision", "ProcessColModels", 0x4185C0, &CCollision::ProcessColModels);
    //Install("CCollision", "SphereCastVsEntity", 0x419F00, &CCollision::SphereCastVsEntity);
    //Install("CCollision", "SphereVsEntity", 0x41A5A0, &CCollision::SphereVsEntity);
    //Install("CCollision", "CheckCameraCollisionBuildings", 0x41A820, &CCollision::CheckCameraCollisionBuildings);
    //Install("CCollision", "CheckCameraCollisionVehicles", 0x41A990, &CCollision::CheckCameraCollisionVehicles);
    //Install("CCollision", "CheckCameraCollisionObjects", 0x41AB20, &CCollision::CheckCameraCollisionObjects);
    //Install("CCollision", "BuildCacheOfCameraCollision", 0x41AC40, &CCollision::BuildCacheOfCameraCollision);
    //Install("CCollision", "CameraConeCastVsWorldCollision", 0x41B000, &CCollision::CameraConeCastVsWorldCollision);

#ifdef _DEBUG
    Tests();
#endif
}

void CCollision::Tests() {
    const auto seed = time(nullptr);
    srand(seed);
    std::cout << "CCollision::Tests seed: " << seed << std::endl;

    const auto VectorEq = [](const CVector& lhs, const CVector& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
    };

    const auto ColPointEq = [&](const CColPoint& lhs, const CColPoint& rhs) {
        return VectorEq(lhs.m_vecPoint, rhs.m_vecPoint)
            && lhs.m_fDepth == rhs.m_fDepth
            && VectorEq(lhs.m_vecNormal, rhs.m_vecNormal);
    };

    const auto RandomVector = [](float min = -100.f, float max = 100.f) {
        return CVector{
            CGeneral::GetRandomNumberInRange(min, max),
            CGeneral::GetRandomNumberInRange(min, max),
            CGeneral::GetRandomNumberInRange(min, max)
        };
    };

    const auto RandomSphere = [&](float min = -100.f, float max = 100.f) {
        return CColSphere{ RandomVector(min, max), CGeneral::GetRandomNumberInRange(min, max) };
    };

    const auto RandomBox = [&](float min = -100.f, float max = 100.f) {
        CColBox cb{ RandomVector(min, max), RandomVector(min, max) };
        cb.Recalc();
        return cb;
    };

    const auto Test = [](auto name, auto org, auto rev, auto cmp, auto&&... args) {
        const auto orgResult = org(args...);
        const auto revResult = rev(args...);
        if (!cmp(orgResult, revResult)) {
            std::cerr << "[CCollision::Tests]: " << name << " failed. " << std::endl;
            assert(0);
        }
    };

    {
        auto sp1 = RandomSphere(), sp2 = RandomSphere();
        auto Original = plugin::CallAndReturn<bool, 0x411E70, CColSphere const&, CColSphere const&>;
        Test("TestSphereSphere", Original, TestSphereSphere, std::equal_to<>{}, sp1, sp2);
    }

    {
        const auto Org = [&](auto box, auto point) {
            CColPoint cp{};
            plugin::Call<0x411EC0, CBox const&, CVector const&, CColPoint&>(box, point, cp);
            return cp;
        };

        const auto Rev = [](auto box, auto point) {
            CColPoint cp{};
            CalculateColPointInsideBox(box, point, cp);
            return cp;
        };

        Test("CalculateColPointInsideBox", Org, Rev, ColPointEq, RandomBox(), RandomVector());
    }

    {
        const auto Org = plugin::CallAndReturn<bool, 0x4120C0, CSphere const&, CBox const&>;
        Test("TestSphereBox", Org, TestSphereBox, std::equal_to{}, RandomSphere(), RandomBox());
    }
}

// 0x411E20
void CCollision::Update() {
    // empty
}

// 0x411E30
void CCollision::SortOutCollisionAfterLoad() {
    CColStore::LoadCollision(TheCamera.GetPosition(), false);
    CStreaming::LoadAllRequestedModels(false);
}

// 0x411E70
bool CCollision::TestSphereSphere(CColSphere const& sphere1, CColSphere const& sphere2) {
    return (sphere1.m_vecCenter - sphere2.m_vecCenter).SquaredMagnitude() <= sphere1.m_fRadius + sphere2.m_fRadius;
}

// 0x411EC0
void CalculateColPointInsideBox(CBox const& box, CVector const& point, CColPoint& colPoint) {
    const auto pointToCenter = box.GetCenter() - point;

    // Calculate component-wise distance to the closest corner (either box.min or box.max)
    #define DoAxis(a) pointToCenter.a <= 0 ? point.a - box.m_vecMin.a : box.m_vecMax.a - point.a
    const CVector pointToClosest{ DoAxis(x), DoAxis(y), DoAxis(z) };
    #undef DoAxis

    const auto CalcNormal = [](float a) {
        return a <= 0.f ? -1.f : 1.f;
    };

    colPoint = {};
    colPoint.m_vecPoint = point;

    if (pointToClosest.x >= pointToClosest.y || pointToClosest.x >= pointToClosest.z) {
        if (pointToClosest.y >= pointToClosest.x || pointToClosest.y >= pointToClosest.z) {
            colPoint.m_vecNormal.z = CalcNormal(pointToClosest.z);
            colPoint.m_fDepth = pointToClosest.z;
        } else {
            colPoint.m_vecNormal.y = CalcNormal(pointToClosest.y);
            colPoint.m_fDepth = pointToClosest.y;
        }
    } else {
        colPoint.m_vecNormal.x = CalcNormal(pointToClosest.x);
        colPoint.m_fDepth = pointToClosest.x;
    }
}

// 0x4120C0
bool CCollision::TestSphereBox(CSphere const& sphere, CBox const& box) {
    const auto IsValueInRange = [](float min, float max, float val) {
        return val >= min && val <= max;
    };
#define CheckAxis(a) (sphere.m_vecCenter.a + sphere.m_fRadius >= box.m_vecMin.a && \
                     sphere.m_vecCenter.a - sphere.m_fRadius <= box.m_vecMax.a)
    return CheckAxis(x) && CheckAxis(y) && CheckAxis(z);
#undef CheckAxis
}

// 0x412130
bool CCollision::ProcessSphereBox(CColSphere const& sphere, CColBox const& box, CColPoint& colPoint, float& maxTouchDistance) {
    return plugin::CallAndReturn<bool, 0x412130, CColSphere const&, CColBox const&, CColPoint&, float&>(sphere, box, colPoint, maxTouchDistance);
}

// 0x412700
bool CCollision::PointInTriangle(CVector const& point, CVector const* triPoints) {
    return plugin::CallAndReturn<bool, 0x412700, CVector const&, CVector const*>(point, triPoints);
}

// 0x412850
float CCollision::DistToLineSqr(CVector const* lineStart, CVector const* lineEnd, CVector const* point) {
    return plugin::CallAndReturn<float, 0x412850, CVector const*, CVector const*, CVector const*>(lineStart, lineEnd, point);
}

// 0x412970
float CCollision::DistToMathematicalLine(CVector const* lineStart, CVector const* lineEnd, CVector const* point) {
    const auto l = *lineEnd - *lineStart;
    const auto p = *point - *lineStart;

    // Simple Pythagorean here, we gotta find side `a`

    const auto cSq = p.SquaredMagnitude();
    const auto bSq = std::pow(DotProduct(p, l), 2) / p.SquaredMagnitude(); // Must divide it by either `l.SquaredMagnitude()` because neither vectors are normalized

    const auto aSq = cSq - bSq;
    return bSq > 0.0f ? std::sqrt(bSq) : 0.0f; // Little optimization to not call `sqrt` if the dist is 0 (it wont ever be negative)
}

// 0x412A30
float CCollision::DistToMathematicalLine2D(float lineStartX, float lineStartY, float lineEndX, float lineEndY, float pointX, float pointY) {
    return plugin::CallAndReturn<float, 0x412A30, float, float, float, float, float, float>(lineStartX, lineStartY, lineEndX, lineEndY, pointX, pointY);
}

// 0x412A80
float CCollision::DistAlongLine2D(float lineX, float lineY, float lineDirX, float lineDirY, float pointX, float pointY) {
    return plugin::CallAndReturn<float, 0x412A80, float, float, float, float, float, float>(lineX, lineY, lineDirX, lineDirY, pointX, pointY);
}

// 0x412AA0
bool CCollision::ProcessLineSphere(CColLine const& line, CColSphere const& sphere, CColPoint& colPoint, float& depth) {
    return plugin::CallAndReturn<bool, 0x412AA0, CColLine const&, CColSphere const&, CColPoint&, float&>(line, sphere, colPoint, depth);
}

// 0x412C70
bool CCollision::TestLineBox_DW(CColLine const& line, CBox const& box) {
    return plugin::CallAndReturn<bool, 0x412C70, CColLine const&, CBox const&>(line, box);
}

// 0x413070
bool CCollision::TestLineBox(CColLine const& line, CBox const& box) {
    return plugin::CallAndReturn<bool, 0x413070, CColLine const&, CBox const&>(line, box);
}

// 0x413080
bool CCollision::TestVerticalLineBox(CColLine const& line, CBox const& box) {
    return plugin::CallAndReturn<bool, 0x413080, CColLine const&, CBox const&>(line, box);
}

// 0x413100
bool CCollision::ProcessLineBox(CColLine const& line, CColBox const& box, CColPoint& colPoint, float& maxTouchDistance) {
    return plugin::CallAndReturn<bool, 0x413100, CColLine const&, CColBox const&, CColPoint&, float&>(line, box, colPoint, maxTouchDistance);
}

// 0x4138D0
bool CCollision::Test2DLineAgainst2DLine(float line1StartX, float line1StartY, float line1EndX, float line1EndY, float line2StartX, float line2StartY, float line2EndX, float line2EndY) {
    return plugin::CallAndReturn<bool, 0x4138D0, float, float, float, float, float, float, float, float>(line1StartX, line1StartY, line1EndX, line1EndY, line2StartX, line2StartY, line2EndX, line2EndY);
}

// 0x413960
bool ProcessDiscCollision(CColPoint& colPoint1, CMatrix const& mat, CColDisk const& disk, CColPoint& colPoint2, bool& arg4, float& arg5, CColPoint& colPoint3) {
    return plugin::CallAndReturn<bool, 0x413960, CColPoint&, CMatrix const&, CColDisk const&, CColPoint&, bool&, float&, CColPoint&>(colPoint1, mat, disk, colPoint2, arg4, arg5, colPoint3);
}

// 0x413AC0
bool CCollision::TestLineTriangle(CColLine const& line, CompressedVector const* verts, CColTriangle const& tri, CColTrianglePlane const& triPlane) {
    return plugin::CallAndReturn<bool, 0x413AC0, CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&>(line, verts, tri, triPlane);
}

// 0x4140F0
bool CCollision::ProcessLineTriangle(CColLine const& line, CompressedVector const* verts, CColTriangle const& tri, CColTrianglePlane const& triPlane, CColPoint& colPoint, float& maxTouchDistance, CStoredCollPoly* collPoly) {
    return plugin::CallAndReturn<bool, 0x4140F0, CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*>(line, verts, tri, triPlane, colPoint, maxTouchDistance, collPoly);
}

// 0x4147E0
bool CCollision::ProcessVerticalLineTriangle(CColLine const& line, CompressedVector const* verts, CColTriangle const& tri, CColTrianglePlane const& triPlane, CColPoint& colPoint, float& maxTouchDistance, CStoredCollPoly* collPoly) {
    return plugin::CallAndReturn<bool, 0x4147E0, CColLine const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&, CStoredCollPoly*>(line, verts, tri, triPlane, colPoint, maxTouchDistance, collPoly);
}

// 0x414D70
bool CCollision::IsStoredPolyStillValidVerticalLine(CVector const& lineOrigin, float lineDist, CColPoint& colPoint, CStoredCollPoly* collPoly) {
    return plugin::CallAndReturn<bool, 0x414D70, CVector const&, float, CColPoint&, CStoredCollPoly*>(lineOrigin, lineDist, colPoint, collPoly);
}

// 0x415230
void CCollision::GetBoundingBoxFromTwoSpheres(CColBox* bbox, CColSphere* sphere1, CColSphere* sphere2) {
    plugin::Call<0x415230, CColBox*, CColSphere*, CColSphere*>(bbox, sphere1, sphere2);
}

// 0x4152C0
bool CCollision::IsThisVehicleSittingOnMe(CVehicle* vehicle, CVehicle* vehicleOnMe) {
    return plugin::CallAndReturn<bool, 0x4152C0, CVehicle*, CVehicle*>(vehicle, vehicleOnMe);
}

// 0x415320
bool CCollision::CheckCameraCollisionPeds(int32 sectorX, int32 sectorY, CVector* pos, CVector* dir, float* arg4) {
    return plugin::CallAndReturn<bool, 0x415320, int32, int32, CVector*, CVector*, float*>(sectorX, sectorY, pos, dir, arg4);
}

// 0x4154A0
bool CCollision::CheckPeds(CVector* pos, CVector* dir, float* arg2) {
    return plugin::CallAndReturn<bool, 0x4154A0, CVector*, CVector*, float*>(pos, dir, arg2);
}

// 0x415540
void ResetMadeInvisibleObjects() {
    plugin::Call<0x415540>();
}

// 0x415590
bool CCollision::SphereCastVsBBox(CColSphere* sphere1, CColSphere* sphere2, CColBox* box) {
    return plugin::CallAndReturn<bool, 0x415590, CColSphere*, CColSphere*, CColBox*>(sphere1, sphere2, box);
}

// 0x415620
bool CCollision::RayPolyPOP(CVector* arg0, CVector* arg1, CColTriangle* arg2, CVector* arg3, CVector* arg4) {
    return plugin::CallAndReturn<bool, 0x415620, CVector*, CVector*, CColTriangle*, CVector*, CVector*>(arg0, arg1, arg2, arg3, arg4);
}

// 0x4156D0
int32 CCollision::GetPrincipleAxis(CVector* vec) {
    return plugin::CallAndReturn<int32, 0x4156D0, CVector*>(vec);
}

// 0x415730
bool CCollision::PointInPoly(CVector* point, CColTriangle* tri, CVector* arg2, CVector* triVerts) {
    return plugin::CallAndReturn<bool, 0x415730, CVector*, CColTriangle*, CVector*, CVector*>(point, tri, arg2, triVerts);
}

// 0x415950
void CCollision::Closest3(CVector* arg0, CVector* arg1) {
    plugin::Call<0x415950, CVector*, CVector*>(arg0, arg1);
}

// 0x415A40
float ClosestSquaredDistanceBetweenFiniteLines(CVector* line1Start, CVector* line1End, CVector* line2Start, CVector* line2End, float arg4) {
    return plugin::CallAndReturn<float, 0x415A40, CVector*, CVector*, CVector*, CVector*, float>(line1Start, line1End, line2Start, line2End, arg4);
}

// 0x415CF0
bool CCollision::SphereCastVersusVsPoly(CColSphere* sphere1, CColSphere* sphere2, CColTriangle* tri, CColTrianglePlane* triPlane, CompressedVector* verts) {
    return plugin::CallAndReturn<bool, 0x415CF0, CColSphere*, CColSphere*, CColTriangle*, CColTrianglePlane*, CompressedVector*>(sphere1, sphere2, tri, triPlane, verts);
}

// 0x416260
void CCollision::Init() {
    plugin::Call<0x416260>();
}

// 0x4162E0
void CCollision::Shutdown() {
    plugin::Call<0x4162E0>();
}

// 0x416330
void CCollision::CalculateTrianglePlanes(CCollisionData* colData) {
    if (!colData->m_nNumTriangles)
        return;

    if (colData->m_pTrianglePlanes)
    {
        auto* pLink = colData->GetLinkPtr();
        pLink->Remove();
        CCollision::ms_colModelCache.usedListHead.Insert(pLink);
    }
    else
    {
        auto* pLink = CCollision::ms_colModelCache.Insert(colData);
        if (!pLink)
        {
            auto* pToRemove = CCollision::ms_colModelCache.usedListTail.prev;
            pToRemove->data->RemoveTrianglePlanes();
            CCollision::ms_colModelCache.Remove(pToRemove);
            pLink = CCollision::ms_colModelCache.Insert(colData);
        }

        colData->CalculateTrianglePlanes();
        colData->SetLinkPtr(pLink);
    }
}

// 0x416400
void CCollision::RemoveTrianglePlanes(CCollisionData* colData) {
    if (!colData->m_pTrianglePlanes)
        return;

    auto* pLink = colData->GetLinkPtr();
    CCollision::ms_colModelCache.Remove(pLink);
    colData->RemoveTrianglePlanes();
}

// 0x416450
bool CCollision::ProcessSphereSphere(CColSphere const& sphere1, CColSphere const& sphere2, CColPoint& colPoint, float& maxTouchDistance) {
    return plugin::CallAndReturn<bool, 0x416450, CColSphere const&, CColSphere const&, CColPoint&, float&>(sphere1, sphere2, colPoint, maxTouchDistance);
}

// 0x4165B0
bool CCollision::TestSphereTriangle(CColSphere const& sphere, CompressedVector const* verts, CColTriangle const& tri, CColTrianglePlane const& triPlane) {
    return plugin::CallAndReturn<bool, 0x4165B0, CColSphere const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&>(sphere, verts, tri, triPlane);
}

// 0x416BA0
bool CCollision::ProcessSphereTriangle(CColSphere const& sphere, CompressedVector const* verts, CColTriangle const& tri, CColTrianglePlane const& triPlane, CColPoint& colPoint, float& maxTouchDistance) {
    return plugin::CallAndReturn<bool, 0x416BA0, CColSphere const&, CompressedVector const*, CColTriangle const&, CColTrianglePlane const&, CColPoint&, float&>(sphere, verts, tri, triPlane, colPoint, maxTouchDistance);
}

// 0x417470
bool CCollision::TestLineSphere(CColLine const& line, CColSphere const& sphere) {
    return plugin::CallAndReturn<bool, 0x417470, CColLine const&, CColSphere const&>(line, sphere);
}

// 0x417610
float CCollision::DistToLine(CVector const& lineStart, CVector const& lineEnd, CVector const& point) {
    return plugin::CallAndReturn<float, 0x417610, CVector const&, CVector const&, CVector const&>(lineStart, lineEnd, point);
}

// 0x417730
bool CCollision::TestLineOfSight(CColLine const& line, CMatrix const& transform, CColModel& colModel, bool doSeeThroughCheck, bool doShootThroughCheck) {
    return plugin::CallAndReturn<bool, 0x417730, CColLine const&, CMatrix const&, CColModel&, bool, bool>(line, transform, colModel, doSeeThroughCheck, doShootThroughCheck);
}

// 0x417950
bool CCollision::ProcessLineOfSight(CColLine const& line, CMatrix const& transform, CColModel& colModel, CColPoint& colPoint, float& maxTouchDistance, bool doSeeThroughCheck, bool doShootThroughCheck) {
    return plugin::CallAndReturn<bool, 0x417950, CColLine const&, CMatrix const&, CColModel&, CColPoint&, float&, bool, bool>(line, transform, colModel, colPoint, maxTouchDistance, doSeeThroughCheck, doShootThroughCheck);
}

// 0x417BF0
bool CCollision::ProcessVerticalLine(CColLine const& line, CMatrix const& transform, CColModel& colModel, CColPoint& colPoint, float& maxTouchDistance, bool doSeeThroughCheck, bool doShootThroughCheck, CStoredCollPoly* collPoly) {
    return plugin::CallAndReturn<bool, 0x417BF0, CColLine const&, CMatrix const&, CColModel&, CColPoint&, float&, bool, bool, CStoredCollPoly*>(line, transform, colModel, colPoint, maxTouchDistance, doSeeThroughCheck, doShootThroughCheck, collPoly);
}

// 0x417F20
bool CCollision::SphereCastVsSphere(CColSphere* arg0, CColSphere* arg1, CColSphere* arg2) {
    return plugin::CallAndReturn<bool, 0x417F20, CColSphere*, CColSphere*, CColSphere*>(arg0, arg1, arg2);
}

// 0x417FD0
void CCollision::ClosestPointOnLine(CVector* arg0, CVector* arg1, CVector* arg2, CVector* arg3) {
    plugin::Call<0x417FD0, CVector*, CVector*, CVector*, CVector*>(arg0, arg1, arg2, arg3);
}

// 0x418100
void CCollision::ClosestPointsOnPoly(CColTriangle* arg0, CVector* arg1, CVector* arg2, CVector* arg3) {
    plugin::Call<0x418100, CColTriangle*, CVector*, CVector*, CVector*>(arg0, arg1, arg2, arg3);
}

// 0x418150
void CCollision::ClosestPointOnPoly(CColTriangle* arg0, CVector* arg1, CVector* arg2) {
    plugin::Call<0x418150, CColTriangle*, CVector*, CVector*>(arg0, arg1, arg2);
}

// 0x4181B0
bool CCollision::SphereCastVsCaches(CColSphere* sphere, CVector* arg1, int32 arg2, CColCacheEntry* arg3, int32* arg4, CColCacheEntry* arg5) {
    return plugin::CallAndReturn<bool, 0x4181B0, CColSphere*, CVector*, int32, CColCacheEntry*, int32*, CColCacheEntry*>(sphere, arg1, arg2, arg3, arg4, arg5);
}

// 0x418580
void CCollision::CalculateTrianglePlanes(CColModel* colModel) {
    plugin::Call<0x418580, CColModel*>(colModel);
}

// 0x4185A0
void CCollision::RemoveTrianglePlanes(CColModel* colModel) {
    plugin::Call<0x4185A0, CColModel*>(colModel);
}

// 0x4185C0
int32 CCollision::ProcessColModels(CMatrix const& transform1, CColModel& colModel1, CMatrix const& transform2, CColModel& colModel2, CColPoint* colPoint1, CColPoint* colPoint2, float* maxTouchDistance, bool arg7) {
    return plugin::CallAndReturn<int32, 0x4185C0, CMatrix const&, CColModel&, CMatrix const&, CColModel&, CColPoint*, CColPoint*, float*, bool>(transform1, colModel1, transform2, colModel2, colPoint1, colPoint2, maxTouchDistance, arg7);
}

// 0x419F00
bool CCollision::SphereCastVsEntity(CColSphere* sphere1, CColSphere* sphere2, CEntity* entity) {
    return plugin::CallAndReturn<bool, 0x419F00, CColSphere*, CColSphere*, CEntity*>(sphere1, sphere2, entity);
}

// 0x41A5A0
bool CCollision::SphereVsEntity(CColSphere* sphere, CEntity* entity) {
    return plugin::CallAndReturn<bool, 0x41A5A0, CColSphere*, CEntity*>(sphere, entity);
}

// 0x41A820
bool CCollision::CheckCameraCollisionBuildings(int32 sectorX, int32 sectorY, CColBox* arg2, CColSphere* arg3, CColSphere* arg4, CColSphere* arg5) {
    return plugin::CallAndReturn<bool, 0x41A820, int32, int32, CColBox*, CColSphere*, CColSphere*, CColSphere*>(sectorX, sectorY, arg2, arg3, arg4, arg5);
}

// 0x41A990
bool CCollision::CheckCameraCollisionVehicles(int32 sectorX, int32 sectorY, CColBox* arg2, CColSphere* arg3, CColSphere* arg4, CColSphere* arg5, CVector* arg6) {
    return plugin::CallAndReturn<bool, 0x41A990, int32, int32, CColBox*, CColSphere*, CColSphere*, CColSphere*, CVector*>(sectorX, sectorY, arg2, arg3, arg4, arg5, arg6);
}

// 0x41AB20
bool CCollision::CheckCameraCollisionObjects(int32 sectorX, int32 sectorY, CColBox* arg2, CColSphere* arg3, CColSphere* arg4, CColSphere* arg5) {
    return plugin::CallAndReturn<bool, 0x41AB20, int32, int32, CColBox*, CColSphere*, CColSphere*, CColSphere*>(sectorX, sectorY, arg2, arg3, arg4, arg5);
}

// 0x41AC40
bool CCollision::BuildCacheOfCameraCollision(CColSphere* sphere1, CColSphere* sphere2) {
    return plugin::CallAndReturn<bool, 0x41AC40, CColSphere*, CColSphere*>(sphere1, sphere2);
}

// 0x41B000
bool CCollision::CameraConeCastVsWorldCollision(CColSphere* sphere1, CColSphere* sphere2, float* arg2, float arg3) {
    return plugin::CallAndReturn<bool, 0x41B000, CColSphere*, CColSphere*, float*, float>(sphere1, sphere2, arg2, arg3);
}
