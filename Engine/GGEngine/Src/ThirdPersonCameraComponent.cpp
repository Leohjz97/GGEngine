#include "Precompiled.h"
#include "ThirdPersonCameraComponent.h"

#include "CameraComponent.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "TransformComponent.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

void ThirdPersonCameraComponent::Initialize()
{
    cameraComponent = GetOwner().GetComponent<CameraComponent>();
}

//ToDo
//Tips: have the next camera position
// use lerp to move the camerea from the current position to the new position
// can be use a camera distance to do this

void ThirdPersonCameraComponent::Update(float deltaTime)
{
    auto obj = GetOwner().GetWorld().FindGameObjectByName(mTarget);
    if (obj)
    {
        const auto transformComponent = obj->GetComponent<TransformComponent>();
        const auto transform = transformComponent->GetMatrix4();
        const auto pos = GetTranslation(transform);
        const auto distance = GetLookAt(transform) * (-mDistance);
        const auto cameraHeight = Vector3::YAxis * mCameraHeight;
        const auto lookheight = Vector3::YAxis * mLookHeight;

        auto& cam = cameraComponent->GetCamera();
        cam.SetPosition(pos + distance + cameraHeight + NewCamera);
        cam.SetLookAt(pos + lookheight);
    }
}

void ThirdPersonCameraComponent::DebugUI()
{
    if (ImGui::CollapsingHeader("Third Person Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Distance", &mDistance, 1.0, -10.0, 800.0f);
        ImGui::DragFloat("Camera Height", &mCameraHeight, 1.0, -10.0, 800.0f);
        ImGui::DragFloat("Look Height", &mLookHeight, 1.0, -10.0, 800.0f);
        ImGui::DragFloat("Camera Position X",&NewCamera.x, 1.0, -10.0, 800.0f);
        ImGui::DragFloat("Camera Position Y", &NewCamera.y, 1.0, -10.0, 800.0f);
        ImGui::DragFloat("Camera Position Z", &NewCamera.z, 1.0, -10.0, 800.0f);
    }
}

void GGEngine::ThirdPersonCameraComponent::ParseThirdPersonCameraComponent(const rj::Value& data, GameObject& obj)
{
    auto mThirdPersonCam = obj.AddComponent<ThirdPersonCameraComponent>();

    if (data.HasMember("Target"))
    {
        const auto& name = data["Target"].GetString();
        mThirdPersonCam->SetTarget(name);
    }

    if (data.HasMember("Distance"))
    {
        const auto& distance = data["Distance"].GetFloat();
        mThirdPersonCam->SetDistance(distance);
    }
    if (data.HasMember("CameraHeight"))
    {
        const auto& camHeight = data["CameraHeight"].GetFloat();
        mThirdPersonCam->SetCameraHeight(camHeight);
    }
    if (data.HasMember("LookHeight"))
    {
        const auto& lookHeight = data["LookHeight"].GetFloat();
        mThirdPersonCam->SetLookHeight(lookHeight);
    }
}
