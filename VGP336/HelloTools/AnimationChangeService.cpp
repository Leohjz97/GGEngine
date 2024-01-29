#include "AnimationChangeService.h"
#include "AnimationChangeComoponent.h"
#include<Input/Inc/InputSystem.h>

using namespace GGEngine;
using namespace GGEngine::Input;

void AnimationChangeService::Initialize()
{

}

void AnimationChangeService::Terminate()
{
    mAnimationChangeComponents.clear();
}

void AnimationChangeService::Update(float deltaTime)
{
	auto input = GGEngine::Input::InputSystem::Get();
	if (input->IsKeyPressed(KeyCode::UP))
	{
		SetAllAnimationIdx(1);
	}
	if (input->IsKeyPressed(KeyCode::DOWN))
	{
		SetAllAnimationIdx(2);
	}
	if (input->IsKeyPressed(KeyCode::LEFT))
	{
		SetAllAnimationIdx(3);
	}
	if (input->IsKeyPressed(KeyCode::RIGHT))
	{
		SetAllAnimationIdx(4);
	}
}

void AnimationChangeService::SetAllAnimationIdx(int idx)
{
	for (auto animChangeComponet : mAnimationChangeComponents)
	{
		animChangeComponet->SetAnimation(idx);
	}
}

void AnimationChangeService::Register(AnimationChangeComponent* animationChangeComponent)
{
	if (std::find(mAnimationChangeComponents.begin(), mAnimationChangeComponents.end(), animationChangeComponent) == mAnimationChangeComponents.end())
	{
		mAnimationChangeComponents.push_back(animationChangeComponent);
	}
}

void AnimationChangeService::Unregister(const AnimationChangeComponent* animationChangeComponent)
{
	auto iter = std::find(mAnimationChangeComponents.begin(), mAnimationChangeComponents.end(), animationChangeComponent);
	if (iter != mAnimationChangeComponents.end())
	{
		mAnimationChangeComponents.erase(iter);
	}
}
