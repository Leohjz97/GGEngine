#include"Precompiled.h"
#include "Colors.h"
#include "SimpleDraw.h"
#include "AnimationUtil.h"
#include "Animtor.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::Graphic::AnimationUtil;

namespace
{
	void ComputeBoneTransformRecursive(const Bone* bone, BoneTransforms& boneTransforms, const Animator* animator)
	{
		if (bone != nullptr)
		{
			if (animator != nullptr)
			{
				boneTransforms[bone->idx] = animator->GetParentTransfor(bone);
			}
			else
			{
				boneTransforms[bone->idx] = bone->toParentTransform;
			}

			if (bone -> parent != nullptr)
			{
				boneTransforms[bone->idx] = boneTransforms[bone->idx] * boneTransforms[bone->parentIdx];
			}
			else
			{
				boneTransforms[bone->idx] = bone->toParentTransform;
			}

			for (auto child : bone->children)
			{
				ComputeBoneTransformRecursive(child, boneTransforms, animator);
			}
		}
	}
}

void AnimationUtil::ComputeBoneTransform(ModelID id, BoneTransforms& bonetransform,const Animator* animator)
{
	auto model = ModelManager::Get()->GetModel(id);
	if (model->mSkeleton != nullptr)
	{
		bonetransform.resize(model->mSkeleton->bones.size(), Matrix4::Identity);
		ComputeBoneTransformRecursive(model->mSkeleton->root, bonetransform, animator);
	}
}

void AnimationUtil::ApplyBoneOffsets(ModelID id, BoneTransforms& bonetransform)
{
	auto model = ModelManager::Get()->GetModel(id);
	if (model->mSkeleton != nullptr)
	{
		for (auto& bone: model->mSkeleton->bones)
		{
			bonetransform[bone->idx] = bone->offsetTransform * bonetransform[bone->idx];
		}
	}
}

void AnimationUtil::DrawSkeleton(ModelID id, const BoneTransforms& bonetransform)
{
	auto model = ModelManager::Get()->GetModel(id);
	if (model->mSkeleton != nullptr)
	{
		for (auto& bone : model->mSkeleton->bones)
		{
			if (bone->parent != nullptr)
			{
				const auto posA = GetTranslation(bonetransform[bone->idx]);
				const auto posB = GetTranslation(bonetransform[bone->parentIdx]);
				SimpleDraw::AddLine(posA, posB, Colors::MediumVioletRed);
			}
		}
	}
}
