#pragma once
#include "KeyFrame.h"
#include "Transform.h"

namespace GGEngine::Graphic
{
    class Animation
    {
    public:
        GGMath::Vector3 GetPosition(float time) const;
        GGMath::Quaternion GetRotation(float time) const;
        GGMath::Vector3 GetScale(float time) const;


        Transform GetTransform(float time)const;

        float GetDuration() const;

    private:
        friend class AnimationBuilder;
        friend class AnimationIO;

        PositionKeys mPositionKeys;
        RotationKeys mRotationKeys;
        ScaleKeys mScaleKeys;
        float mDuration;
    };
}