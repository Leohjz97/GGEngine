#pragma once
#include "Component.h"
namespace rj = rapidjson;
namespace GGEngine
{
    class CameraComponent;
    class TransformComponent;

    class ThirdPersonCameraComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::ThirdPersonCamera);

        void Initialize() override;
        void Update(float deltaTime) override;
        void DebugUI() override;

        void ParseThirdPersonCameraComponent(const rj::Value& data, GameObject& obj);

        void SetTarget(std::string name) { mTarget = std::move(name); }
        void SetDistance(float distance) { mDistance = distance; }
        void SetCameraHeight(float height) { mCameraHeight = height; }
        void SetLookHeight(float height) { mLookHeight = height; }

    private:
        CameraComponent* cameraComponent = nullptr;

        std::string mTarget;
        float mDistance = 0.0f;
        float mCameraHeight = 0.0f;
        float mLookHeight = 0.0f;
        Vector3 NewCamera;
    };
}