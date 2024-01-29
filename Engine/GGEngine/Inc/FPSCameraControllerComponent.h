#pragma once
#include "Component.h"
namespace rj = rapidjson;

namespace GGEngine
{
    class CameraComponent;
    class FPSCameraControllerComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::FPSCameraController);

        void Initialize() override;
        void Terminate() override;
        void Update(float deltaTime) override;

        void ParseFPSCameraComponent(const rj::Value& data, GameObject& obj);

        void SetMoveSpeed(float speed) { mMoveSpeed = speed; }
        void SetTurnSpeed(float speed) { mTurnSpeed = speed; }

    private:
        CameraComponent* cam = nullptr;
        float mMoveSpeed = 1.0f;
        float mTurnSpeed = 0.0f;
    };

}
