#include "Precompiled.h"
#include "RenderObject.h"
#include "Model.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void RenderObject::Terminate()
{
    diffuesMapID = 0;
    specularMapID = 0;
    displacementMapID = 0;
    normalMapID = 0;
    modelid = 0;
    animator = nullptr;
    meshBuffer.Terminate();
}
RenderGroup Graphic::CreateRenderGroup(const Model& model, const Animator* animator)
{
    RenderGroup renderGroup;
    renderGroup.reserve(model.meshData.size());

    auto TryLoadTexture = [](const auto& textureName)->textureID
    {
        if (textureName.empty())
        {
            return 0;
        }
        return TextureManger::Get()->LoadTexture(textureName, false);
    };

    for (auto& meshData : model.meshData)
    {
        auto& renderObj = renderGroup.emplace_back();
        auto& materialData = model.materialData[meshData.materialIdx];
        renderObj.material = materialData.material;
        renderObj.diffuesMapID = TryLoadTexture(materialData.diffuseMapName);
        renderObj.specularMapID = TryLoadTexture(materialData.specularMapName);
        renderObj.displacementMapID = TryLoadTexture(materialData.displacementMapName);
        renderObj.normalMapID = TryLoadTexture(materialData.normalMapName);

        //Skeleton
        renderObj.skeleton = model.mSkeleton.get();
        renderObj.animator = animator;

        renderObj.meshBuffer.Initialize(meshData.mesh);
    }

    return renderGroup;
}

RenderGroup Graphic::CreateRenderGroup(ModelID id, const Animator* animator)
{
    auto model = ModelManager::Get()->GetModel(id);
    RenderGroup rendergroup = CreateRenderGroup(*model, animator);
    for (auto& renderObj : rendergroup)
    {
        renderObj.modelid = id;
    }
    return rendergroup;
}

void Graphic::CleanupRenderGroup(RenderGroup& renderGroup)
{
    for (auto& renderObj : renderGroup)
    {
        renderObj.Terminate();
    }
}