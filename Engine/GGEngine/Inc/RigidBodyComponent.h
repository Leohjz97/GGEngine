#pragma once
#include "Component.h"

namespace rj = rapidjson;
namespace GGEngine
{
    class RigidBodyComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::RigidBodyComponet);

        void Initialize() override;
        void Terminate() override;

        void SetMass(float mass);
        void SetPosition(const GGEngine::GGMath::Vector3& pos);

        void ParseRigidBodyComponent(const rj::Value& data, GameObject& obj);
    private:
        Physics::RigidBody rb;
        float mMass = 0.0f;
    };
}