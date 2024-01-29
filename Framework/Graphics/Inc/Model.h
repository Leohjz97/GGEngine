#pragma once
#include "AnimationClip.h"
#include "MeshType.h"
#include "Material.h"
#include "Skeleton.h"

namespace GGEngine::Graphic
{
    struct Model
    {
        struct MeshData
        {
            Mesh mesh;
            uint32_t materialIdx = 0;
        };

        struct MaterialData
        {
            Material material;
            std::string diffuseMapName;
            std::string specularMapName;
            std::string displacementMapName;
            std::string normalMapName;
        };

        std::vector<MeshData> meshData;
        std::vector<MaterialData> materialData;
        std::unique_ptr<Skeleton> mSkeleton;
        std::vector<AnimationClip> mAnimationClips;
    };

}