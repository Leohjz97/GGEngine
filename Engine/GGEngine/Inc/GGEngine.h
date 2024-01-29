#pragma once
#include "Common.h"
#include "App.h"
#include "AppState.h"
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "GameObjectHandle.h"
#include "GameWorld.h"
#include "Service.h"
#include "TypeID.h"

//Components
#include "AnimatorComponent.h"
#include "CameraComponent.h"
#include "CharacterContollerComponent.h"
#include "ColliderComponent.h"
#include "RigidBodyComponent.h"
#include "TransformComponent.h"
#include "ThirdPersonCameraComponent.h"
#include "FPSCameraControllerComponent.h"
#include "ModelComponent.h"
#include "MeshComponent.h"

//Service
#include "CameraService.h"
#include "RenderService.h"

namespace GGEngine
{
    App& MainApp();
}