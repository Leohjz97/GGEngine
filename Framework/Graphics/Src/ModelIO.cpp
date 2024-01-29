#include "Precompiled.h"
#include "ModelIO.h"
#include "Model.h"
#include "Material.h"
#include "AnimationBuilder.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void AnimationIO::Write(FILE* file, const Animation& anim)
{
	uint32_t count = anim.mPositionKeys.size();
	fprintf_s(file, "PositionKeysCount: %d\n", count);
	for (auto& k : anim.mPositionKeys)
	{
		fprintf_s(file, "%f %f %f %f\n", k.time, k.key.x, k.key.y, k.key.z);
	}

	count = anim.mRotationKeys.size();
	fprintf_s(file, "RotationKeysCount: %d\n", count);
	for (auto& k : anim.mRotationKeys)
	{
		fprintf_s(file, "%f %f %f %f %f\n", k.time, k.key.x, k.key.y, k.key.z, k.key.w);
	}

	count = anim.mScaleKeys.size();
	fprintf_s(file, "ScaleKeysCount: %d\n", count);
	for (auto& k : anim.mScaleKeys)
	{
		fprintf_s(file, "%f %f %f %f\n", k.time, k.key.x, k.key.y, k.key.z);
	}
}
void AnimationIO::Read(FILE* file, Animation& anim)
{
	AnimationBuilder build;

	uint32_t numPositionKeys,numRotationKeys,numScaleKeys;
	fscanf_s(file, "PositionKeysCount: %d\n", &numPositionKeys);
	for (uint32_t k =0;k< numPositionKeys; ++k)
	{
		float time = 0.0f;
		Vector3 pos;
		fscanf_s(file, "%f %f %f %f\n", &time, &pos.x, &pos.y, &pos.z);
		build.AddPositionKey(pos, time);
	}

	fscanf_s(file, "RotationKeysCount: %d\n", &numRotationKeys);
	for (uint32_t k = 0; k < numRotationKeys; ++k)
	{
		float time = 0.0f;
		Quaternion rot;
		fscanf_s(file, "%f %f %f %f %f\n", &time, &rot.x, &rot.y, &rot.z, &rot.w);
		build.AddRotationKey(rot, time);
	}

	fscanf_s(file, "ScaleKeysCount: %d\n", &numScaleKeys);
	for (uint32_t k = 0; k < numScaleKeys; ++k)
	{
		float time = 0.0f;
		Vector3 scale;
		fscanf_s(file, "%f %f %f %f\n", &time, &scale.x, &scale.y, &scale.z);
		build.AddScaleKey(scale, time);
	}

	anim = build.Build();
}
void ModelIO::SaveModel(std::filesystem::path filePath, const Model& model)
{
	if (model.meshData.empty())
	{
		return;
	}

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == NULL)
	{
		return;
	}

	const uint32_t meshCount = static_cast<uint32_t>(model.meshData.size());
	fprintf_s(file, "MeshCount: %d\n", meshCount);
	for (uint32_t i = 0; i < meshCount; ++i)
	{
		const auto& meshData = model.meshData[i];
		fprintf_s(file, "MaterialIdx: %d\n", meshData.materialIdx);

		const auto& mesh = meshData.mesh;
		const uint32_t vertexCount = static_cast<uint32_t>(mesh.verticies.size());
		fprintf_s(file, "VertexCount: %d\n", vertexCount);

		for (auto& vtx : mesh.verticies)
		{
			fprintf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d %d %d\n",
				vtx.position.x, vtx.position.y, vtx.position.z,
				vtx.normal.x, vtx.normal.y, vtx.normal.z,
				vtx.tangent.x, vtx.tangent.y, vtx.tangent.z,
				vtx.uvCoord.x, vtx.uvCoord.y,
				vtx.boneWeights[0], vtx.boneWeights[1], vtx.boneWeights[2], vtx.boneWeights[3],
				vtx.boneIndices[0], vtx.boneIndices[1], vtx.boneIndices[2], vtx.boneIndices[3]
			);
		}

		const uint32_t idxCount = static_cast<uint32_t>(mesh.indicies.size());
		fprintf_s(file, "Index Count: %d\n", idxCount);
		for (size_t n = 2; n < idxCount; n += 3)
		{
			fprintf_s(file, "%d %d %d\n", mesh.indicies[n - 2], mesh.indicies[n - 1], mesh.indicies[n]);
		}
	}

	fclose(file);
}

void ModelIO::LoadModel(std::filesystem::path filePath, Model& model)
{
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == NULL)
	{
		return;
	}

	uint32_t meshCount = 0;
	fscanf_s(file, "MeshCount: %d\n", &meshCount);

	model.meshData.resize(meshCount);

	for (uint32_t i = 0; i < meshCount; ++i)
	{
		auto& meshData = model.meshData[i];
		fscanf_s(file, "MaterialIdx: %d\n", &meshData.materialIdx);

		auto& mesh = meshData.mesh;
		uint32_t vertexCount = 0;
		fscanf_s(file, "VertexCount: %d\n", &vertexCount);
		mesh.verticies.resize(vertexCount);

		for (auto& vtx : mesh.verticies)
		{
			fscanf_s(file, "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d %d %d %d\n",
				&vtx.position.x, &vtx.position.y, &vtx.position.z,
				&vtx.normal.x,   &vtx.normal.y,   &vtx.normal.z,
				&vtx.tangent.x,  &vtx.tangent.y,  &vtx.tangent.z,
				&vtx.uvCoord.x,  &vtx.uvCoord.y,
				&vtx.boneWeights[0], &vtx.boneWeights[1], &vtx.boneWeights[2], &vtx.boneWeights[3],
				&vtx.boneIndices[0], &vtx.boneIndices[1], &vtx.boneIndices[2], &vtx.boneIndices[3]
			);
		}

		uint32_t idxCount = 0;
		fscanf_s(file, "Index Count: %d\n", &idxCount);
		mesh.indicies.resize(idxCount);
		for(size_t n = 2; n < idxCount; n += 3)
		{
			fscanf_s(file, "%d %d %d\n", &mesh.indicies[n - 2], &mesh.indicies[n - 1], &mesh.indicies[n]);
		}
	}

	fclose(file);
}

void ModelIO::SaveMaterial(std::filesystem::path filePath, const Model& model)
{
	if (model.materialData.empty())
	{
		return;
	}
	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == NULL)
	{
		return;
	}

	uint32_t materialCount = static_cast<uint32_t>(model.materialData.size());
	fprintf_s(file, "MatericalCount: %d\n", materialCount);
	for (auto& materialData: model.materialData)
	{
		auto& md = materialData.material;
		fprintf_s(file, "%f %f %f %f\n", md.ambient.r, md.ambient.g, md.ambient.b, md.ambient.a);
		fprintf_s(file, "%f %f %f %f\n", md.diffuse.r, md.diffuse.g, md.diffuse.b, md.diffuse.a);
		fprintf_s(file, "%f %f %f %f\n", md.specular.r, md.specular.g, md.specular.b, md.specular.a);
		fprintf_s(file, "%f %f %f %f\n", md.emissive.r, md.emissive.g, md.emissive.b, md.emissive.a);
		fprintf_s(file, "SpecularPower: %f\n", md.power);

		fprintf_s(file, "%s\n", materialData.diffuseMapName.empty() ? "none" : materialData.diffuseMapName.c_str());
		fprintf_s(file, "%s\n", materialData.specularMapName.empty() ? "none" : materialData.specularMapName.c_str());
		fprintf_s(file, "%s\n", materialData.displacementMapName.empty() ? "none" : materialData.displacementMapName.c_str());
		fprintf_s(file, "%s\n", materialData.normalMapName.empty() ? "none" : materialData.normalMapName.c_str());
	}
}

void ModelIO::LoadMaterial(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("material");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == NULL)
	{
		return;
	}

	auto TryReadTextureName = [&](auto& fileName)
	{
		char buffer[MAX_PATH]{};
		fscanf_s(file,"%s\n", buffer, (uint32_t)sizeof(buffer));
		if (strcmp(buffer, "none") != 0)
		{
			fileName = filePath.replace_filename(buffer).string();
		}
	};

	uint32_t materialCount = 0;
	fscanf_s(file, "MatericalCount: %d\n", &materialCount);
	model.materialData.resize(materialCount);

	for (auto& materialData : model.materialData)
	{
		auto& md = materialData.material;
		fscanf_s(file, "%f %f %f %f\n", &md.ambient.r,  &md.ambient.g,  &md.ambient.b,  &md.ambient.a);
		fscanf_s(file, "%f %f %f %f\n", &md.diffuse.r,  &md.diffuse.g,  &md.diffuse.b,  &md.diffuse.a);
		fscanf_s(file, "%f %f %f %f\n", &md.specular.r, &md.specular.g, &md.specular.b, &md.specular.a);
		fscanf_s(file, "%f %f %f %f\n", &md.emissive.r, &md.emissive.g, &md.emissive.b, &md.emissive.a);
		fscanf_s(file, "SpecularPower: %f\n", &md.power);

		TryReadTextureName(materialData.diffuseMapName);
		TryReadTextureName(materialData.specularMapName);
		TryReadTextureName(materialData.displacementMapName);
		TryReadTextureName(materialData.normalMapName);
	}

	fclose(file);
}

void ModelIO::SaveSkeleton(std::filesystem::path filePath, const Model& model)
{
	if (model.mSkeleton == nullptr)
	{
		return;
	}
	filePath.replace_extension("skeleton");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
	{
		return;
	}

	auto WriteMatrix = [&](auto& m)
	{
		fprintf_s(file, "%f %f %f %f\n", m._11, m._12, m._13, m._14);
		fprintf_s(file, "%f %f %f %f\n", m._21, m._22, m._23, m._24);
		fprintf_s(file, "%f %f %f %f\n", m._31, m._32, m._33, m._34);
		fprintf_s(file, "%f %f %f %f\n", m._41, m._42, m._43, m._44);
	};

	uint32_t boneCount = model.mSkeleton->bones.size();
	fprintf_s(file, "RootBone: %d\n", model.mSkeleton->root->idx);
	fprintf_s(file, "BoneCount: %d\n", boneCount);

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		auto& boneData = model.mSkeleton->bones[i];
		fprintf_s(file, "BoneName: %s\n", boneData->name.c_str());
		fprintf_s(file, "BoneIdx: %d\n", boneData->idx);
		fprintf_s(file, "BoneParentIdx: %d\n", boneData->parentIdx);

		uint32_t childrenCount = boneData->childrenIndices.size();
		fprintf_s(file, "BoneChildrenCount: %d\n", childrenCount);
		for (uint32_t c = 0; c  < childrenCount; ++c)
		{
			fprintf_s(file, "%d\n", boneData->childrenIndices[c]);
		}

		WriteMatrix(boneData->toParentTransform);
		WriteMatrix(boneData->offsetTransform);
	}
	fclose(file);
}

void ModelIO::LoadSkeleton(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("skeleton");
	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
	{
		return;
	}

	auto ReadMatrix = [&](auto& m)
	{
		fscanf_s(file, "%f %f %f %f\n", &m._11, &m._12, &m._13, &m._14);
		fscanf_s(file, "%f %f %f %f\n", &m._21, &m._22, &m._23, &m._24);
		fscanf_s(file, "%f %f %f %f\n", &m._31, &m._32, &m._33, &m._34);
		fscanf_s(file, "%f %f %f %f\n", &m._41, &m._42, &m._43, &m._44);
	};

	model.mSkeleton = std::make_unique<Skeleton>();

	int boneCount = 0;
	int rootIdx = 0;
	fscanf_s(file, "RootBone: %d\n", &rootIdx);
	fscanf_s(file, "BoneCount: %d\n", &boneCount);

	model.mSkeleton->bones.resize(boneCount);
	for (uint32_t i = 0; i < boneCount; ++i)
	{
		model.mSkeleton->bones[i] = std::make_unique<Bone>();
	}
	model.mSkeleton->root = model.mSkeleton->bones[rootIdx].get();

	for (uint32_t i = 0; i < boneCount; ++i)
	{
		uint32_t boneIdx = 0;
		char boneName[MAX_PATH]{};
		fscanf_s(file, "BoneName: %s\n", boneName, (uint32_t)sizeof(boneName));
		fscanf_s(file, "BoneIdx: %d\n", &boneIdx);

		auto& boneData = model.mSkeleton->bones[boneIdx];
		boneData->name = std::move(boneName);
		boneData->idx = boneIdx;

		fscanf_s(file, "BoneParentIdx: %d\n", &boneData->parentIdx);
		if (boneData->parentIdx >= 0)
		{
			boneData->parent = model.mSkeleton->bones[boneData->parentIdx].get();
		}

		uint32_t childrenCount = 0;
		fscanf_s(file, "BoneChildrenCount: %d\n", &childrenCount);
		if (childrenCount > 0)
		{
			boneData->children.resize(childrenCount);
			boneData->childrenIndices.resize(childrenCount);
			for (uint32_t c = 0; c < childrenCount; ++c)
			{
				uint32_t childIdx = 0;
				fscanf_s(file, "%d\n", &childIdx);
				boneData->childrenIndices[c] = childIdx;
				boneData->children[c] = model.mSkeleton->bones[childIdx].get();
			}
		}

		ReadMatrix(boneData->toParentTransform);
		ReadMatrix(boneData->offsetTransform);
	}
	fclose(file);
}

void ModelIO::SaveAnimations(std::filesystem::path filePath, const Model& model)
{
	if (model.mSkeleton == nullptr || model.mAnimationClips.empty())
	{
		return;
	}
	filePath.replace_extension("animset");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "w");
	if (file == nullptr)
	{
		return;
	}

	uint32_t animClipCount = model.mAnimationClips.size();
	fprintf_s(file, "AnimationClipCount: %d\n", animClipCount);
	for (uint32_t i = 0; i < animClipCount; ++i)
	{
		auto& animClipData = model.mAnimationClips[i];
		fprintf_s(file, "AnimationClipName: %s\n", animClipData.name.c_str());
		fprintf_s(file, "TickDuration: %f\n", animClipData.tickDuration);
		fprintf_s(file, "TickPerSecond: %f\n", animClipData.tickPerSecond);

		uint32_t boneAnimCount = animClipData.boneAnimations.size();
		fprintf_s(file, "BoneAnimCount: %d\n", boneAnimCount);
		for (uint32_t b = 0; b < boneAnimCount; ++b)
		{
			auto boneAnim = animClipData.boneAnimations[b].get();
			if (boneAnim == nullptr)
			{
				fprintf_s(file, "[EMPTY]\n");
				continue;
			}

			fprintf_s(file, "[ANIMATION]\n");
			//uint32_t count=boneAnim.m
			AnimationIO::Write(file, *boneAnim);
		}
	}

	fclose(file);
}

void ModelIO::LoadAnimations(std::filesystem::path filePath, Model& model)
{
	filePath.replace_extension("animset");

	FILE* file = nullptr;
	fopen_s(&file, filePath.u8string().c_str(), "r");
	if (file == nullptr)
	{
		return;
	}

	uint32_t animClipCount = 0;
	fscanf_s(file, "AnimationClipCount: %d\n", &animClipCount);

	for (uint32_t i = 0; i < animClipCount; ++i)
	{
		auto& animClipData = model.mAnimationClips.emplace_back();

		char animClipName[MAX_PATH]{};

		fscanf_s(file, "AnimationClipName: %s\n", animClipName, (uint32_t)sizeof(animClipName));
		animClipData.name = std::move(animClipName);

		fscanf_s(file, "TickDuration: %f\n", &animClipData.tickDuration);
		fscanf_s(file, "TickPerSecond: %f\n", &animClipData.tickPerSecond);

		uint32_t boneAnimCount = 0;
		fscanf_s(file, "BoneAnimCount: %d\n", &boneAnimCount);
		animClipData.boneAnimations.resize(boneAnimCount);
		for (uint32_t b = 0; b < boneAnimCount; ++b)
		{
			char label[128]{};
			fscanf_s(file, "%s\n", label,(uint32_t)sizeof(label));
			if (strcmp(label,"[ANIMATION]")==0)
			{
				animClipData.boneAnimations[b] = std::make_unique<Animation>();
				AnimationIO::Read(file, *animClipData.boneAnimations[b]);
			}
		}
	}

	fclose(file);
}
