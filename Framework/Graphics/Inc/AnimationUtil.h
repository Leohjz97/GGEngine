#pragma once
//#include "Skeleton.h"
//
//using namespace GGEngine::GGMath;

#include "ModelManager.h"
#include "Animtor.h"

namespace GGEngine::Graphic::AnimationUtil
{
    using BoneTransforms = std::vector<Matrix4>;

    void ComputeBoneTransform(ModelID id, BoneTransforms& bonetransform, const Animator* animtor = nullptr);
    void ApplyBoneOffsets(ModelID id, BoneTransforms& bonetransform);
    void DrawSkeleton(ModelID id, const BoneTransforms& bonetransform);
}