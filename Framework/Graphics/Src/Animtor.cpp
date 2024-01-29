#include "Precompiled.h"
#include "Animtor.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;

void Animator::Initialize(ModelID ID)
{
    mModelID = ID;
}

void Animator::PlayAnimation(int clipIdx, bool looping, float blendDuration)
{
    if (mCurrentAnimation.GetAnimationTick() == -1 || blendDuration <= 0.0f)
    {
        mCurrentAnimation.PlayAnimation(mModelID, clipIdx, looping);
    }
    else
    {
        mNextAnimation.PlayAnimation(mModelID, clipIdx, looping);
        mBlendDuration = blendDuration;
        mBlendTime = 0.0f;
    }
}

void Animator::Update(float deltTime)
{
    if (mCurrentAnimation.GetClipeIdx() != -1)
    {
        mCurrentAnimation.Update(deltTime);
    }
    if (mNextAnimation.GetClipeIdx() != -1)
    {
        mNextAnimation.Update(deltTime);
    }

    if (mBlendDuration > 0.0f && mBlendDuration > mBlendTime)
    {
        mBlendTime += deltTime;
        if (mBlendTime >= mBlendDuration)
        {
            mBlendTime = 0.0f;
            mBlendDuration = 0.0f;
            mCurrentAnimation = std::move(mNextAnimation);
        }
    }
}

bool Animator::isFinished()
{
    return mCurrentAnimation.IsFinished();
}

size_t Animator::GetAnimationCount() const
{
    auto model = ModelManager::Get()->GetModel(mModelID);
    return model->mAnimationClips.size();
}

Matrix4 Animator::GetParentTransfor(const Bone* bone) const
{
    Transform fromTransform;

    if (!mCurrentAnimation.GetToParentTransform(bone,fromTransform))
    {
        return bone->toParentTransform;
    }

    if (mNextAnimation.GetClipeIdx() != -1)
    {
        float t = Clamp(mBlendTime / mBlendDuration, 0.0f, 1.0f);
        Transform toTransform;
        if (!mNextAnimation.GetToParentTransform(bone,toTransform))
        {
            return bone->toParentTransform;
        }
        
        Transform blendedTransform;
        blendedTransform.position = Lerp(fromTransform.position, toTransform.position, t);
        blendedTransform.rotation = Quaternion::Slerp(fromTransform.rotation, toTransform.rotation, t);
        blendedTransform.scale = Lerp(fromTransform.scale, toTransform.scale, t);

        return blendedTransform.GetMatrix4();
    }
    return fromTransform.GetMatrix4();
}

void BlendNode::PlayAnimation(ModelID id, int clipIdx, bool looping)
{
    mModelId = id;
    mClipIdx = clipIdx;
    mLooping = looping;
    mAnimationTick = 0.0f;
}

void BlendNode::Update(float deltTime)
{
    auto model = ModelManager::Get()->GetModel(mModelId);
    const auto& animClip = model->mAnimationClips[mClipIdx];
    mAnimationTick += animClip.tickPerSecond * deltTime;
    
    if (mAnimationTick > animClip.tickDuration)
    {
        if (mLooping)
        {
            while (mAnimationTick >= animClip.tickDuration)
            {
                mAnimationTick -= animClip.tickDuration;
            }
        }
        else
        {
            mAnimationTick = animClip.tickDuration;
        }
    }
}

bool BlendNode::GetToParentTransform(const Bone* bone, Transform& transform) const
{
    auto model = ModelManager::Get()->GetModel(mModelId);
    const auto& animClip = model->mAnimationClips[mClipIdx];
    auto& animation = animClip.boneAnimations[bone->idx];
    if (animation == nullptr)
    {
        return false;
    }
    transform = animation->GetTransform(mAnimationTick);
    return true;
}

bool GGEngine::Graphic::BlendNode::IsFinished() const
{
    if (mLooping || mClipIdx < 0)
    {
        return false;
    }

    auto model = ModelManager::Get()->GetModel(mModelId);
    const auto& animClip = model->mAnimationClips[mClipIdx];
    return mAnimationTick >= animClip.tickDuration;

}

BlendNode::BlendNode(BlendNode&& os) noexcept
    : mModelId (os.mModelId),
      mClipIdx (os.mClipIdx),
      mAnimationTick (os.mAnimationTick),
      mLooping (os.mLooping)
{
    os.mModelId = 0;
    os.mClipIdx = -1;
    os.mAnimationTick = 0.0f;
    os.mLooping = false;
}

BlendNode& GGEngine::Graphic::BlendNode::operator=(BlendNode&& os) noexcept
{

    mModelId = os.mModelId;
    mClipIdx = os.mClipIdx;
    mAnimationTick = os.mAnimationTick;
    mLooping = os.mLooping;

    os.mModelId = 0;
    os.mClipIdx = -1;
    os.mAnimationTick = 0.0f;
    os.mLooping = false;

    return *this;
}

void BlendedAnimator::PlayAnimation(int clipIdx, bool looping, float blendDuration)
{

}

void BlendedAnimator::SetAnimation(int clipIdx, BlendDirection direction)
{
    mBlendNode[(int)direction].PlayAnimation(mModelID, clipIdx, true);
}

void BlendedAnimator::UpdateWeight(float x,float y)
{
    mWeightX = GGMath::Clamp(x, -1.0f, 1.0f);
    mWeightY = GGMath::Clamp(y, -1.0f, 1.0f);
}

void BlendedAnimator::Update(float deltTime)
{
    for (uint32_t i=0;i<5;++i)
    {
        if (mBlendNode[i].GetClipeIdx() != -1)
        {
            mBlendNode[i].Update(deltTime);
        }
    }
}

GGMath::Matrix4 BlendedAnimator::GetParentTransfor(const Bone* bone) const
{
    Transform finalTran;
    if (mBlendNode[(int)BlendDirection::Center].GetToParentTransform(bone,finalTran))
    {
        if (mWeightY > 0.1f)
        {
            Transform northT;
            if (mBlendNode[(int)BlendDirection::N].GetToParentTransform(bone, finalTran))
            {
                float t = (mWeightY - 0.1f) / 0.9f;
                finalTran.position = Lerp(finalTran.position, northT.position, t);
                finalTran.rotation = Quaternion::Slerp(finalTran.rotation, northT.rotation, t);
                finalTran.scale = Lerp(finalTran.scale, northT.scale, t);
            }
        }
        else if (mWeightY < 0.1f)
        {
            Transform southT;
            if (mBlendNode[(int)BlendDirection::S].GetToParentTransform(bone, finalTran))
            {
                float t = (-mWeightY - 0.1f) / 0.9f;
                finalTran.position = Lerp(finalTran.position, southT.position, t);
                finalTran.rotation = Quaternion::Slerp(finalTran.rotation, southT.rotation, t);
                finalTran.scale = Lerp(finalTran.scale, southT.scale, t);
            }
        }
        else if (mWeightX > 0.1f)
        {
            Transform eastT;
            if (mBlendNode[(int)BlendDirection::E].GetToParentTransform(bone, finalTran))
            {
                float t = (mWeightY - 0.1f) / 0.9f;
                finalTran.position = Lerp(finalTran.position, eastT.position, t);
                finalTran.rotation = Quaternion::Slerp(finalTran.rotation, eastT.rotation, t);
                finalTran.scale = Lerp(finalTran.scale, eastT.scale, t);
            }
        }
        else if (mWeightX < 0.1f)
        {
            Transform westT;
            if (mBlendNode[(int)BlendDirection::W].GetToParentTransform(bone, finalTran))
            {
                float t = (-mWeightY - 0.1f) / 0.9f;
                finalTran.position = Lerp(finalTran.position, westT.position, t);
                finalTran.rotation = Quaternion::Slerp(finalTran.rotation, westT.rotation, t);
                finalTran.scale = Lerp(finalTran.scale, westT.scale, t);
            }
        }
    }

    return GGMath::Matrix4::Identity;

}
