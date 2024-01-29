#pragma once
#include "Material.h"
#include "MeshBuffer.h"
#include "TextureManger.h"
#include "Transform.h"
#include "Skeleton.h"
#include "ModelManager.h"
#include "Animtor.h"

namespace GGEngine::Graphic
{
    struct Model;
    //class Animator;
    class RenderObject
    {
    public: 
        void Terminate();

        Transform trasf;
        
        Material material;

        textureID diffuesMapID;
        textureID normalMapID;
        textureID displacementMapID;
        textureID specularMapID;

        ModelID modelid = 0;
        MeshBuffer meshBuffer;
        const Skeleton* skeleton = nullptr;
        const Animator* animator = nullptr;
    };

    using RenderGroup = std::vector<RenderObject>;
    [[nodiscard]] RenderGroup CreateRenderGroup(const Model& model, const Animator* animator=nullptr);
    [[nodiscard]] RenderGroup CreateRenderGroup(ModelID id, const Animator* animator = nullptr);
    void CleanupRenderGroup(RenderGroup& renderGroup);

    template<class Effect>
    void DrawRenderGroup(Effect& effect, const RenderGroup& renderGroup)
    {
        for ( auto& renderObj : renderGroup)
        {
            effect.Render(renderObj);
        }
    }
}