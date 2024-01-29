#pragma once

#include"ModelManager.h"
#include"Bone.h"

namespace GGEngine::Graphic
{
    class BlendNode
    {
    public:
        BlendNode() = default;
        //delet copy
        BlendNode(BlendNode&) = delete;
        BlendNode& operator=(BlendNode&) = delete;

        //overider 
        BlendNode (BlendNode&& os) noexcept;
        BlendNode& operator=(BlendNode&& os) noexcept;
        
        void PlayAnimation(ModelID id, int clipIdx, bool looping);
        void Update(float deltTime);
        bool GetToParentTransform(const Bone* bone, Transform& transfor)const;
        
        bool IsFinished() const;
        float GetAnimationTick() const { return mAnimationTick; }
        bool IsLooping() const { return mLooping; }
        int GetClipeIdx() const { return mClipIdx; }

    private:
        ModelID mModelId = 0;
        int mClipIdx = -1;
        float mAnimationTick = 0.0f;
        bool mLooping = false;
    };

    class Animator
    {
    public:
        void Initialize(ModelID ID);
        virtual void PlayAnimation(int clipIdx, bool looping, float blendDuration = 0.0f);
        virtual void Update(float deltTime);
        bool isFinished();

        size_t GetAnimationCount() const;
        virtual GGMath::Matrix4 GetParentTransfor(const Bone* bone) const;
        
    protected:
        ModelID mModelID = 0;
        float mBlendDuration = -1.0f;
        float mBlendTime = -1.0f;

        BlendNode mCurrentAnimation;
        BlendNode mNextAnimation;

    };

    class BlendedAnimator :public Animator
    {
    public:
        enum  class BlendDirection
        {
            Center,
            N,
            S,
            E,
            W,

            Count
        };

        void PlayAnimation(int clipIdx, bool looping, float blendDuration = 0.0f);
        void SetAnimation(int clipIdx, BlendDirection direction);
        
        void UpdateWeight(float x, float y);
        void Update(float deltTime);
        
        GGMath::Matrix4 GetParentTransfor(const Bone* bone) const;

    private:
        BlendNode mBlendNode[5];
        float mWeightX = 0.0f;
        float mWeightY = 0.0f;
    };
}