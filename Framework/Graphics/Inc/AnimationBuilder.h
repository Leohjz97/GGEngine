#pragma once
#include "Animation.h"

namespace GGEngine::Graphic
{
    class AnimationBuilder
    {
    public:
        AnimationBuilder& AddPositionKey(const GGMath::Vector3& pos, float t, EaseType easeType = EaseType::Linear);
        AnimationBuilder& AddRotationKey(const GGMath::Quaternion& rotation, float t, EaseType easeType = EaseType::Linear);
        AnimationBuilder& AddScaleKey(const GGMath::Vector3& scale, float t, EaseType easeType = EaseType::Linear);

        [[nodiscard]] Animation Build();

    private:
        Animation mWorkingCopy;
    };
}