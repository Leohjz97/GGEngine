#pragma once

#include"ConstantBuffer.h"
#include"LighType.h"
#include"PixelShader.h"
#include"Sampler.h"
#include"VertexShader.h"
#include "Material.h"

namespace GGEngine::Graphic
{
    class Camera;
    class RenderObject;
    class Texture;

    class StandardEffect
    {
    public:
        void Initialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderobject);

        void SetCamera(const Camera& cam);
        void SetDirectionalLight(const DirectionaLight& light);
        void SetLightCamerea(const Camera& camera);
        void SetShadowMap(const Texture* shadowMap);

        void DebugUI();

    private:
        struct TransformData
        {
            Matrix4 world;
            Matrix4 wvp[2];
            Vector3 viewPostion;
            float displacementWeight;
        };

        struct SettingsData
        {
            int useDiffuseMap = 1;
            int useSpecularMap = 1;
            int useDisplacementMap = 1;
            int useNormalMap = 1;
            int useShadowMap = 1;
            int useSkinning = 1;
            float depthBias = 0.0f;
            float padding[1] = { 0.0f };
        };

        using TranformBuffer = TypedConstantBuffer<TransformData>;
        using BoneTranformBuffer = ConstantBuffer;
        using LightBuffer = TypedConstantBuffer<DirectionaLight>;
        using MaterialBuffer = TypedConstantBuffer<Material>;
        using SettingsBuffer = TypedConstantBuffer<SettingsData>;

        TranformBuffer mTransferbuffer;
        BoneTranformBuffer mBoneTransferbuffer;
        LightBuffer mLightBuffer;
        MaterialBuffer mMatericalBuffer;
        SettingsBuffer mSettingsBuffer;
        Samlper mSamlper;

        VertexShader mVertexShader;
        PiexlShader mPiexlShader;

        SettingsData mSettingsData;

        float mDisplacementWeight = 1.0f;
        float mDepthBias = 0.0f;
        const Camera* mCamera = nullptr;
        const Camera* mLightCamera = nullptr;
        const DirectionaLight* mLight = nullptr;
        const Texture* mShadowMap = nullptr;
    };
}