#pragma once
#include <GGEngine/Inc/GGEngine.h>
#include "TypeId.h"

namespace GGEngine
{
    class AnimatorComponent;
}

class AnimationChangeComponent final : public GGEngine::Component
{
public:
    SET_TYPE_ID(NewComponentID::AnimationChangeComponenetID);

    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void SetAnimation(int idx);

private:
    GGEngine::AnimatorComponent* animationComponet = nullptr;
};