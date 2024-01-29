#pragma once

#include "Component.h"
namespace rj = rapidjson;
namespace GGEngine
{

    class AnimatorComponent;
    class TransformComponent;
    class RigidBodyComponent;

    enum CharacterAnimations
    {
        Skeleton,
        Dancing,
        Idle,
        Walk,
        Sprint,
        Left,
        Right,
        Back,

        Count
    };

    class CharacterControllerComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::CharacterController);

        void Initialize() override;

        void Update(float deltaTime) override;
        void DebugUI() override;

        void ParseCharacterControlComponent(const rj::Value& data, GameObject& obj);

        void SetWalkSpeed(float walkSpeed) { mWalkSpeed = walkSpeed; }
        void SetSprintSpeed(float sprintSpeed) { mSprintSpeedMult = sprintSpeed; }

    private:
        TransformComponent* mTransformComp = nullptr;
        AnimatorComponent* mAnimatorComp = nullptr;
        RigidBodyComponent* rb = nullptr;

        float moveSpeed;
        Matrix4 transform;
        Vector3 vel = Vector3::Zero;
        Vector3 newPosition;
        Vector3 newRotation;

        CharacterAnimations currentState = CharacterAnimations::Idle;
        float mTurnSpeed;
        float mWalkSpeed = 600.0f;
        float mSprintSpeedMult = 2.0f;
        float mSprintSpeed = 0.0f;
        float mJumpForce = 2.0f;
    };
}