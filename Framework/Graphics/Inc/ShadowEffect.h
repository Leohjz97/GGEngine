#pragma once
#include "Camera.h"
#include "ConstantBuffer.h"
#include "LighType.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "RenderTarget.h"

namespace GGEngine::Graphic
{
    class RenderObject;
    class ShadowEffect
    {
    public:
        void Initialize();
        void Terminate();

        void Begin();
        void End();

        void Render(const RenderObject& renderobj);
        
        void SetDirectionLight(const DirectionaLight& directionalLight);

        void SetFocus(const GGMath::Vector3& focusPosition) { mFocusPosition = focusPosition; }
        void SetSize(float size) { mSize = size; }

        const Camera& GetLightCam() const { return mLightCam; }
        const Texture& GetDepthMap()const { return mDepthMapRenderTarget; }

        void DebugUI();

    private:
        void UpdateLightCam();

        struct TransformData
        {
            GGMath::Matrix4 wvp;
        };

        using TransformBuffer = TypedConstantBuffer<TransformData>;
        TransformBuffer mTransforBuffer;
        VertexShader mVertexShader;
        PiexlShader mPixelShader;

        const DirectionaLight* mDirectionalLight = nullptr;

        Camera mLightCam;
        RenderTarget mDepthMapRenderTarget;

        GGMath::Vector3 mFocusPosition;
        float mSize = 35.0f;
    };
}