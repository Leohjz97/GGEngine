#pragma once
#include <GGEngine/Inc/GGEngine.h>
#include "TypeId.h"

class AnimationChangeComponent;

class AnimationChangeService final : public GGEngine::Service
{
public:
    SET_TYPE_ID(NewServiceID::AnimationChangeInputServiceID);

    void Initialize() override;
    void Terminate() override;

    void Update(float deltaTime) override;
    void SetAllAnimationIdx(int idx);

private:
    using AnimationChangeComponents = std::vector< AnimationChangeComponent*>;
    AnimationChangeComponents mAnimationChangeComponents;

    friend class AnimationChangeComponent;
    void Register(AnimationChangeComponent* animationChangeComponent);
    void Unregister(const AnimationChangeComponent* animationChangeComponent);
};