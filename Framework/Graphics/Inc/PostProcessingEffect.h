#pragma once
#include "ConstantBuffer.h"
#include "PixelShader.h"
#include"Sampler.h"
#include "VertexShader.h"

namespace GGEngine::Graphic
{
    class RenderObject;
    class Texture;

    class PostProcessingEffect
    {
    public:
        enum class Mode
        {
            None,
            Monochrome,
            Invert,
            Mirror,
            Blur,
            Combine2
        };

        void Intitialize(const std::filesystem::path& filePath);
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& obj);
        void SetTexture(const Texture* tex, uint32_t slot = 0);
        void SetMode(Mode mode);
        void SetMirrorScale(float mirrorScale);

        void DebugUI();

    private:
        struct PostProcessData
        {
            Mode mode = Mode::None;
            float param0 = 0.0f;
            float param1 = 0.0f;
            float param02 = 0.0f;
        };

        using PostProcessBuffer = TypedConstantBuffer<PostProcessData>;
        
        VertexShader mVertexShader;
        PiexlShader mPixelShader;

        PostProcessBuffer mPostProcessBuffer;

        Samlper mSampler;

        std::array<const Texture*, 4> mTextures;
        Mode mMode = Mode::None;
        float mMirrorScale = -1.0f;
    };
}