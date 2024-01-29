#include "Precompiled.h"
#include "CameraComponent.h"
#include "CameraService.h"
#include "GameWorld.h"

using namespace GGEngine;
void CameraComponent::Initialize()
{
    auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
    ASSERT(cameraService != nullptr, "CameraConponent: CameraService is not available");
    cameraService->Register(this);
}
void CameraComponent::Terminate() 
{
    auto cameraService = GetOwner().GetWorld().GetService<CameraService>();
    ASSERT(cameraService != nullptr, "CameraConponent: CameraService is not available");
    cameraService->Unregister(this);
}

void CameraComponent::ParseCameraComponent(const rj::Value& data, GameObject& obj)
{
	CameraComponent* cameraComponent = obj.AddComponent<CameraComponent>();

	if (data.HasMember("Position"))
	{
		const auto& position = data["Position"].GetArray();
		const float x = position[0].GetFloat();
		const float y = position[1].GetFloat();
		const float z = position[2].GetFloat();
		cameraComponent->GetCamera().SetPosition({ x,y,z });
	}

	if (data.HasMember("LookAt"))
	{
		const auto& position = data["LookAt"].GetArray();
		const float x = position[0].GetFloat();
		const float y = position[1].GetFloat();
		const float z = position[2].GetFloat();
		cameraComponent->GetCamera().SetLookAt({ x,y,z });
	}
	return;
}
