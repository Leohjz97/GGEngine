#include "Precompiled.h"
#include "RigidBody.h"
#include "PhysicsWorld.h"
#include "CollisionShape.h"
#include "Graphics/Inc/Transform.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::Physics;

RigidBody::~RigidBody()
{
    ASSERT(rb == nullptr && mMotionState == nullptr, "RigidBody: terminate must be called first");
}

void RigidBody::Initialize(Graphic::Transform& gTransform, const CollisionShape& shape ,float mass)
{
    mGraphicTransform = &gTransform;
    mMass = mass;

    mMotionState = new btDefaultMotionState(ConvertTobtTransform(gTransform));
    rb = new btRigidBody(mMass, mMotionState, shape.GetCollisionShape());

    PhysicsWorld::Get()->Register(this);
}

void RigidBody::Terminate()
{
    PhysicsWorld::Get()->UnRegister(this);

    SafeDeletePhyics(mMotionState);
    SafeDeletePhyics(rb);
}

void RigidBody::SetCollisionFilter(int flag)
{
    rb->setCollisionFlags(flag);
}

void RigidBody::SetPosition(const GGEngine::GGMath::Vector3& pos)
{
    mGraphicTransform->position = pos;
    rb->setWorldTransform(ConvertTobtTransform(*mGraphicTransform));
}

void RigidBody::SetVelocity(const GGEngine::GGMath::Vector3& vel)
{
    rb->setLinearVelocity(ConvertTobtVector3(vel));
}

bool RigidBody::IsDynamic() const
{
    return mMass > 0.0f;
}

void RigidBody::UpdateTransform()
{
    ApplybtTransformToTransform(rb->getWorldTransform(), *mGraphicTransform);
}
