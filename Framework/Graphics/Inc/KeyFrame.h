#pragma once
#include "Common.h"

namespace GGEngine::Graphic
{
    enum EaseType // Sine,Quad,Cubic,Expo,Back,Bounce,Elastic
    {
        Linear,
        None,
        EaseIn,
        EaseOut,
        EaseInOut,
        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,
        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,
        EaseInExpo,
        EaseOutExpo,
        EaseInOutExpo,
        EaseInBack,
        EaseOutBack,
        EaseInOutBack,
        EaseInBounce,
        EaseOutBounce,
        EaseInOutBounce,
        EaseInElastic,
        EaseOutElastic,
        EaseInOutElastic
    };

    template<class T>
    struct KeyFrame
    {
        KeyFrame() = default;
        KeyFrame(const T& k, float t,EaseType e=EaseType::Linear)
            :key(k),time(t),easeType(e)
        {
        
        }

        T key = T();
        float time = 0.0f;
        EaseType easeType = EaseType::Linear;
    };

    template<class T>
    using KeyFrames = std::vector<KeyFrame<T>>;

    using PositionKeys = KeyFrames<GGMath::Vector3>;
    using RotationKeys = KeyFrames<GGMath::Quaternion>;
    using ScaleKeys = KeyFrames<GGMath::Vector3>;
}