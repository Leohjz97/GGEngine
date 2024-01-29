#include "Precompiled.h"
#include "AnimationBuilder.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;

namespace
{
    template<class T>
    inline void PushKey(KeyFrames<T>& keyframes, const T& value, float t, EaseType e)
    {
        ASSERT(keyframes.empty() || keyframes.back().time <= t, 
            "Animation Builder -- Cannot add KeyFrame back in time");
        keyframes.emplace_back(value, t, e);
    }
}

AnimationBuilder& AnimationBuilder::AddPositionKey(const GGMath::Vector3& pos, float t, EaseType easeType)
{
    PushKey(mWorkingCopy.mPositionKeys, pos, t,easeType);
    mWorkingCopy.mDuration = Max(mWorkingCopy.mDuration, t);
    return *this;
}

AnimationBuilder& AnimationBuilder::AddRotationKey(const GGMath::Quaternion& roation, float t, EaseType easeType)
{
    PushKey(mWorkingCopy.mRotationKeys, roation, t, easeType);
    mWorkingCopy.mDuration = Max(mWorkingCopy.mDuration, t);
    return *this;
}

AnimationBuilder& AnimationBuilder::AddScaleKey(const GGMath::Vector3& scale, float t, EaseType easeType)
{
    PushKey(mWorkingCopy.mScaleKeys, scale, t, easeType);
    mWorkingCopy.mDuration = Max(mWorkingCopy.mDuration, t);
    return *this;
}

Animation AnimationBuilder::Build()
{
    ASSERT(!mWorkingCopy.mPositionKeys.empty() ||
        !mWorkingCopy.mRotationKeys.empty() ||
        !mWorkingCopy.mScaleKeys.empty(),
        "Animation Builder -- EMPTY!");

    //ASSERT(mWorkingCopy.mDuration > 0.0f, "Animation Builder -- Animation has no duration");

    return std::move(mWorkingCopy);
}
