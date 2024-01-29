#include "AnimationChangeComoponent.h"
#include <GGEngine/Inc/GameObject.h>
#include <GGEngine/Inc/AnimatorComponent.h>
#include <GGEngine/Inc/GameWorld.h>
#include "AnimationChangeService.h"

void AnimationChangeComponent::Initialize()
{
    animationComponet = GetOwner().GetComponent<GGEngine::AnimatorComponent>();

    auto animationChangeService = GetOwner().GetWorld().GetService<AnimationChangeService>();
    animationChangeService->Register(this);
}

void AnimationChangeComponent::Terminate()
{
    auto animChangeService = GetOwner().GetWorld().GetService<AnimationChangeService>();
    animChangeService->Unregister(this);
    animationComponet = nullptr;
}

void AnimationChangeComponent::Update(float deltaTime)
{
}

void AnimationChangeComponent::SetAnimation(int idx)
{
    animationComponet->GetAnimator().PlayAnimation(idx, true);
}
