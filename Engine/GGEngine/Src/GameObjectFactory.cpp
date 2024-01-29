#include "Precompiled.h"
#include "GameObjectFactory.h"

#include "AnimatorComponent.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "CharacterContollerComponent.h"
#include "ColliderComponent.h"
#include "FPSCameraControllerComponent.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "ThirdPersonCameraComponent.h"
#include "MeshComponent.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;

namespace rj = rapidjson;

//TODO: Clean all if condition make everything easy to read and reference 
//make a readjson fucntion in the component

namespace
{
	CustomMake TryMake;
}

void GGEngine::GameObjectFactory::SetCustomMake(CustomMake customMake)
{
	TryMake = customMake;
}

void GameObjectFactory::Make(const std::filesystem::path& templateFile, GameObject& obj)
{
	FILE* file = nullptr;
	auto err = fopen_s(&file, templateFile.u8string().c_str(), "r");
	ASSERT(err == 0 && file != nullptr, "GameObject Factory: Failed to open templated file %s", templateFile.u8string().c_str());

	char readBuffer[65536];
	rj::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
	fclose(file);

	rj::Document doc;
	doc.ParseStream(readStream);

	// void ParseModelComponent(const rj::Value& data, GameObject& obj);

	auto components = doc["Components"].GetObj();
	for (auto& component: components)
	{
		const char* componentName = component.name.GetString();

		//if (TryMake(componentName,component.value,obj))
		//{
		//	//custom componet use this
		//}
		//else 
		if (strcmp(componentName, "CameraComponent") == 0)
		{
			CameraComponent cam;
			cam.ParseCameraComponent(component.value, obj);
		}
		else if (strcmp(componentName, "FPSCameraControllerComponent") == 0)
		{
			FPSCameraControllerComponent fpsCame;
			fpsCame.ParseFPSCameraComponent(component.value, obj);
		}
		else if (strcmp(componentName, "TransformComponent") == 0)
		{
			TransformComponent* transform = obj.AddComponent<TransformComponent>();
			//TransformComponent transform;
			//transform.ParseTransformComponent(component.value, obj);
		}
		else if (strcmp(componentName, "ModelComponent") == 0)
		{
			ModelComponent modelConponent;
			modelConponent.ParseModelComponent(component.value, obj);
		}
		else if (strcmp(componentName,"AnimatorComponent") == 0)
		{
			auto* animatorComponent = obj.AddComponent<AnimatorComponent>();
			//TODO Read Animation
			//transform.ParseAnimatorComponent(component.value, obj);
		}
		else if (strcmp(componentName, "MeshComponent") == 0)
		{
			MeshComponent meshComponent;
			meshComponent.ParseMeshComponent(component.value, obj);
		}
		else if (strcmp(componentName, "ColliderComponent") == 0)
		{
			ColliderComponent collider;
			collider.ParseColliderComponent(component.value, obj);
		}
		else if (strcmp(componentName, "RigidBodyComponent") == 0)
		{
			RigidBodyComponent rb;
			rb.ParseRigidBodyComponent(component.value, obj);
		}
		else if (strcmp(componentName, "ThirdPersonCameraComponent") == 0) 
		{
			ThirdPersonCameraComponent thirdPersonCam;
			thirdPersonCam.ParseThirdPersonCameraComponent(component.value, obj);
		}
		else if (strcmp(componentName, "CharacterContollerComponent") == 0)
		{
			CharacterControllerComponent characterController;
			characterController.ParseCharacterControlComponent(component.value, obj);
	
		}
		else
		{
			ASSERT(false, "ObjectFactory: %s was not defined", componentName);
		}
	}
}