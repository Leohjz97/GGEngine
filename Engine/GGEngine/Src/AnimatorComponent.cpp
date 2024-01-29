#include "Precompiled.h"
#include "AnimatorComponent.h"
#include "ModelComponent.h"
#include "GameObject.h"

using namespace GGEngine;

void GGEngine::AnimatorComponent::Initialize()
{
    auto modelComponent = GetOwner().GetComponent<ModelComponent>();
    mAnimator.Initialize(modelComponent->GetModelID());
    //mAnimator.PlayAnimation(0, true);
    mAnimator.PlayAnimation(2, true);
}

void GGEngine::AnimatorComponent::Update(float deltaTime)
{
    mAnimator.Update(deltaTime);
}

void GGEngine::AnimatorComponent::DebugUI()
{

}

bool GGEngine::AnimatorComponent::PlayAnimation(int idx, bool looping)
{
    mAnimator.PlayAnimation(idx, looping);
    return true;
}
