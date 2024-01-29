#include "Precompiled.h"
#include "Animation.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;

namespace
{
    float GetLerpTime(float startTime, float endTime, float time,EaseType easeType)
    {
        float t = (time - startTime) / (endTime - startTime);
        float t2,t3;
        switch (easeType)
        {
        case EaseIn:
            break; 
        case EaseOut:
            break;
        case EaseInOut:
            break;
        case EaseInQuad: 
            t = t * t;
            break;
        case EaseOutQuad:
            t = -t * (2.0f - t);
            break;
        case EaseInOutQuad:
            t *= 2.0f;
            if (t < 1.0f)
            {
                t = 0.5f * t * t;
                break;
            }
            t -= 1.0f;
            t = -0.5f * ((t * (t - 2.0f)) - 1.0f);
            break;
        case Linear:
            break;
        case EaseInCubic:
            t = t * t * t;
            break;
        case EaseOutCubic:
            t = (--t) * t * t + 1;
            break;
        case EaseInOutCubic:
            if (t < 0.5f)
            {
                t = 4 * t * t * t;
            }
            else
            {
                t = (--t) * (2 * (--t)) * (2 * t);
            }
            break;
        case EaseInExpo:
            if (t == 0)
            {
                t = t;
            }
            else
            {
                t = pow(2, 10 * (--t));
            }
            break;
        case EaseOutExpo:
            if (t == 1.0f)
            {
                t = t;
            }
            else
            {
                t = 1 - pow(2, -10 * t);
            }
            break;
        case EaseInOutExpo:
            if (t == 0.0f || t == 1.0f)
            {
                t = t;
            }
            else if (t < 0.5f)
            {
                t = 0.5f * pow(2.0f, (20.0f * t) - 10.0f);
            }
            else
            {
                t = 1 - 0.5f * pow(2.0f, (-20.0f * t) - 10.0f);
            }
            break;
        case EaseInBack:
            // y = x^3-x*sin(x*pi)
            t = t * t * t - t * sin(t * Constants::Pi);
            break;
        case EaseOutBack:
            // y = 1-((1-x)^3-(1-x)*sin((1-x)*pi))
            t = 1 - (--t * t * t - t * sin(t * Constants::Pi));
            break;
        case EaseInOutBack:
            if (t < 0.5)
            {
                t2 = 2 * t;
                t = 0.5 * (t2 * t2 * t2 - t2 * sin(t2 * Constants::Pi));
            }
            else
            {
                t3 = (1 - (2 * t - 1));
                t = 0.5 * (1 - (t3 * t3 * t3 - t3 * sin(t3 * Constants::Pi))) + 0.5;
            }
            break;
        case EaseInBounce:
            t = pow(2.0f, 6.0f * (t - 1)) * abs(sin(t * Constants::Pi * 3.5f));
            break;
        case EaseOutBounce:
            t = 1 - pow(2.0f, -6.0f * t) * abs(cos(t * Constants::Pi * 3.5f));
            break;
        case EaseInOutBounce:
            if (t < 0.5f) {
                t = 8 * pow(2, 8 * (t - 1)) * abs(sin(t * Constants::Pi * 7));
            }
            else {
                t = 1 - 8 * pow(2, -8 * t) * abs(sin(t * Constants::Pi * 7));
            }
            break;
        case EaseInElastic:
            t2 = t * t;
            t = t2 * t2 * sin(t * Constants::Pi * 4.5f);
            break;
        case EaseOutElastic:
            t2 = (t - 1) * (t - 1);
            t = 1 - t2 * t2 * cos(t * Constants::Pi * 4.5f);
            break;
        case EaseInOutElastic:
            if (t < 0.45) {
                t2 = t * t;
                return 8 * t2 * t2 * sin(t * Constants::Pi * 9);
            }
            else if (t < 0.55) {
                return 0.5 + 0.75 * sin(t * Constants::Pi * 4);
            }
            else {
                t2 = (t - 1) * (t - 1);
                return 1 - 8 * t2 * t2 * sin(t * Constants::Pi * 9);
            }
            break;
        default:
            break;
        }

        return t;
    }
}

Vector3 Animation::GetPosition(float time) const
{
    if (mPositionKeys.size() == 1)
    {
        return mPositionKeys[0].key;
    }
    for (int i = 0; i < mPositionKeys.size(); ++i)
    {
        if (time < mPositionKeys[i].time)
        {
            if (i > 0)
            {
                float lerpTime = GetLerpTime(mPositionKeys[i - 1].time, mPositionKeys[i].time, time, mPositionKeys[i].easeType);
                return Lerp(mPositionKeys[i - 1].key, mPositionKeys[i].key, lerpTime);
            }
            return mPositionKeys[i].key;
        }

        //if (mPositionKeys.size() > 0)
        //{
        //    return mPositionKeys[mPositionKeys.size() - 1].key;
        //}
    }

    return Vector3::Zero;
}

Quaternion Animation::GetRotation(float time) const
{
    if (mRotationKeys.size() == 1)
    {
        return mRotationKeys[0].key;
    }
    for (int i = 0; i < mRotationKeys.size(); ++i)
    {
        if (time < mRotationKeys[i].time)
        {
            if (i > 0)
            {
                float lerpTime = GetLerpTime(mRotationKeys[i - 1].time, mRotationKeys[i].time, time, mRotationKeys[i].easeType);
                return Quaternion::Slerp(mRotationKeys[i - 1].key, mRotationKeys[i].key, lerpTime);
            }
            return mRotationKeys[i].key;
        }

        //if (mRotationKeys.size() > 0)
        //{
        //    return mRotationKeys[mRotationKeys.size() - 1].key;
        //}
    }

    return Quaternion::Zero;
}

Vector3 Animation::GetScale(float time) const
{
    if (mScaleKeys.size() == 1)
    {
        return mScaleKeys[0].key;
    }
    for (int i = 0; i < mScaleKeys.size(); ++i)
    {
        if (time < mScaleKeys[i].time)
        {
            if (i > 0)
            {
                float lerpTime = GetLerpTime(mScaleKeys[i - 1].time, mScaleKeys[i].time, time, mScaleKeys[i].easeType);
                return Lerp(mScaleKeys[i - 1].key, mScaleKeys[i].key, lerpTime);
            }
            return mScaleKeys[i].key;
        }

        //if (mScaleKeys.size() > 0)
        //{
        //    return mScaleKeys[mScaleKeys.size() - 1].key;
        //}
    }

    return Vector3::One;
}

Transform Animation::GetTransform(float time) const
{
    Transform transform;
    transform.position = GetPosition(time);
    transform.rotation = GetRotation(time);
    transform.scale = GetScale(time);

    return transform;
}

float Animation::GetDuration() const
{
    return mDuration;
}
