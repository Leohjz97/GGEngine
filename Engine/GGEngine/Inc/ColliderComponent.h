#pragma once
#include "Component.h"
#include "TypeID.h"

namespace rj = rapidjson;
namespace GGEngine
{
    class ColliderComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::ColliderComponent);

        void Initialize() override;
        void Terminate() override;

        void SetSphereCollider(float radius);
        void SetBoxCollider(const GGMath::Vector3& halfExtents);
        void SetHullCollider(const GGMath::Vector3& halfExtents, const GGMath::Vector3& origin);

        void ParseColliderComponent(const rj::Value& data, GameObject& obj);

        //void SetData()

    private:
        friend class RigidBodyComponent;
        Physics::CollisionShape* mCollisionShape = nullptr;

    };

}