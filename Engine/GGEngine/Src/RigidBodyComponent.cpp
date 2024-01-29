#include "Precompiled.h"
#include "RigidBodyComponent.h"

#include "GameObject.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

using namespace GGEngine;

void RigidBodyComponent::Initialize()
{
    auto* transform = GetOwner().GetComponent<TransformComponent>();
    auto* collider = GetOwner().GetComponent<ColliderComponent>();
    rb.Initialize(*transform, *collider->mCollisionShape, mMass);
}

void RigidBodyComponent::Terminate()
{
    rb.Terminate();
}

void RigidBodyComponent::SetMass(float mass)
{
    mMass = mass;
}

void RigidBodyComponent::SetPosition(const GGEngine::GGMath::Vector3& pos)
{
    rb.SetPosition(pos);
}

void RigidBodyComponent::ParseRigidBodyComponent(const rj::Value& data, GameObject& obj)
{
    auto* rb = obj.AddComponent<RigidBodyComponent>();
    if (data.HasMember("Mass"))
    {
        const float mass = data["Mass"].GetFloat();
        rb->SetMass(mass);
    }
}
