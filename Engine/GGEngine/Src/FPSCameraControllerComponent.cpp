#include "Precompiled.h"
#include "FPSCameraControllerComponent.h"
#include "GameObject.h"
#include "CameraComponent.h"

using namespace GGEngine;
using namespace GGEngine::Input;

void FPSCameraControllerComponent::Initialize()
{
    cam = GetOwner().GetComponent<CameraComponent>();
    ASSERT(cam != nullptr, "FPSCameraControllerComponent: GameObject does not have a camera component");
}

void FPSCameraControllerComponent::Terminate()
{
    cam = nullptr;
}

void FPSCameraControllerComponent::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f * mMoveSpeed : mMoveSpeed;
    const float turnSpeed = mTurnSpeed;

    auto& mCamera = cam->GetCamera();

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }

    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }

    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}

void FPSCameraControllerComponent::ParseFPSCameraComponent(const rj::Value& data, GameObject& obj)
{
    FPSCameraControllerComponent* fpscam = obj.AddComponent<FPSCameraControllerComponent>();

    if (data.HasMember("MoveSpeed"))
    {
        const float moveSpeed = data["MoveSpeed"].GetFloat();
        fpscam->SetMoveSpeed(moveSpeed);
    }
    if (data.HasMember("TurnSpeed"))
    {
        const float turnSpeed = data["TurnSpeed"].GetFloat();
        fpscam->SetTurnSpeed(turnSpeed);
    }

}
