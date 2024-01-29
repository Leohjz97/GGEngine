#pragma once
#include "Component.h"
#include <Graphics/Inc/MeshType.h>
#include <Graphics/Inc/RenderObject.h>

using namespace GGEngine::Graphic;
namespace rj = rapidjson;

namespace GGEngine
{
    class MeshComponent final : public Component
    {
    public:
        SET_TYPE_ID(ComponentID::MeshComponent);

        void Initialize() override;
        void Terminate() override;

        void CreateCube(float size);
        void CreateSphere(int slices, int rings, float rads);
        void CreatePlane(int cols, int rows, float size);

        void SetDiffuseMap(const char* fileName);
        void SetDisplacementMap(const char* fileName);
        void SetNormalMap(const char* fileName);
        void SetSpecularMap(const char* fileName);

        void SetMaterial(const Material& material);

        void ParseMeshComponent(const rj::Value& data, GameObject& obj);

        const RenderObject& GetRenderObject() const;

    private:
        Mesh mMesh;
        RenderObject mRenderObject;
    };
}