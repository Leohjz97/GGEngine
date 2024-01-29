#include "Precompiled.h"
#include "CollisionShape.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Physics;

CollisionShape::~CollisionShape()
{
    ASSERT(shape == nullptr, "CollisionShape: terminate must be called first");
}

void CollisionShape::Terminate()
{
    SafeDeletePhyics(shape);
}

void CollisionShapeSphere::Initialize(float r)
{
    ASSERT(shape == nullptr, "CollisionShapeSphere: collision shape has alreadu been created");
    shape = new btSphereShape(r);
}

void CollisionShapeBox::Initialize(const GGEngine::GGMath::Vector3& extend)
{
    ASSERT(shape == nullptr, "CollisionShapeBox: collision shape has alreadu been created");
    shape = new btBoxShape(ConvertTobtVector3(extend));
}

void CollisionShapeHull::Initialize(const GGEngine::GGMath::Vector3& halfextend, const GGEngine::GGMath::Vector3& origin)
{
    ASSERT(shape == nullptr, "CollisionShapeBox: collision shape has alreadu been created");
    btConvexHullShape* hullShape = new btConvexHullShape();
    
    std::vector<GGEngine::GGMath::Vector3> points = {
        {-halfextend.x,-halfextend.y,-halfextend.z },
        {-halfextend.x, halfextend.y,-halfextend.z },
        { halfextend.x, halfextend.y,-halfextend.z },
        { halfextend.x,-halfextend.y,-halfextend.z },
        { halfextend.x,-halfextend.y, halfextend.z },
        { halfextend.x, halfextend.y, halfextend.z },
        {-halfextend.x, halfextend.y, halfextend.z },
        {-halfextend.x,-halfextend.y, halfextend.z },
    };

    for (auto& p : points)
    {
        hullShape->addPoint(ConvertTobtVector3(p - origin), false);
    }
    hullShape->recalcLocalAabb();
    
    shape = hullShape;
}
