#pragma once
#include "Component.h"

namespace GGEngine{
    class AnimatorComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::Animator);
        
        void Initialize() override;
        void Update(float deltaTime) override;
        void DebugUI();

        bool PlayAnimation(int idx, bool looping);

        Graphic::Animator& GetAnimator() { return mAnimator; }
        const Graphic::Animator& GetAnimator() const { return mAnimator; }

    private:
        Graphic::Animator mAnimator;
    };
}
