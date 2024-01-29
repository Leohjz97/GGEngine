#include "Precompiled.h"
#include "ModelComponent.h"
#include "GameWorld.h"
#include "RenderService.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;


void ModelComponent::Initialize()
{
    auto mm = ModelManager::Get();
    mModelID = mm->GetModelID(mFileName);
    if (mm->GetModel(mModelID) == nullptr)
    {
        mm->LoadModelID(mFileName);
    
        for (const auto& filename:mAnimationFileNames)
        {
            mm->AddAnimation(mModelID, filename);
        }
    }

    auto renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->Register(this);
}

void ModelComponent::Terminate()
{
    auto renderService = GetOwner().GetWorld().GetService<RenderService>();
    renderService->UnRegister(this);
}

void ModelComponent::ParseModelComponent(const rj::Value& data, GameObject& obj)
{
    ModelComponent* modelConponent = obj.AddComponent<ModelComponent>();
    if (data.HasMember("FileName"))
    {
        const char* fileName = data["FileName"].GetString();
        modelConponent->SetFileName(fileName);
    }

    if (data.HasMember("Animations"))
    {
        const auto& animationNames = data["Animations"].GetArray();
        for (const auto& animationName : animationNames)
        {
            const char* animation = animationName.GetString();
            modelConponent->AddAnimation(animation);
        }
    }
    if (data.HasMember("Rotation"))
    {
        const auto& rotation = data["Rotation"].GetArray();
        const float x = rotation[0].GetFloat() * GGMath::Constants::DegToRad;
        const float y = rotation[1].GetFloat() * GGMath::Constants::DegToRad;
        const float z = rotation[2].GetFloat() * GGMath::Constants::DegToRad;
        modelConponent->SetRotation({ x,y,z });
    }
}
