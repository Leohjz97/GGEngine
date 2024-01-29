#include "Precompiled.h"
#include "ShadowEffect.h"
#include "Camera.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void ShadowEffect::Initialize()
{
    mLightCam.SetMode(Camera::ProjectionMode::Orthographic);

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Shadow.fx";

    mVertexShader.Initialize<Vertex>(shaderFile);
    mPixelShader.Initialize(shaderFile);

    mTransforBuffer.Initialize();

    constexpr uint32_t depthMapResolution = 4096;
    mDepthMapRenderTarget.Initialize(depthMapResolution, depthMapResolution,Texture::Format::RGBA_U32);
}

void ShadowEffect::Terminate()
{
    mDepthMapRenderTarget.Terminate();
    mTransforBuffer.Terminate();
    mVertexShader.Terminate();
    mPixelShader.Terminate();
}

void ShadowEffect::Begin()
{
    UpdateLightCam();

    mVertexShader.Bind();
    mPixelShader.Bind();
    mTransforBuffer.BindVS(0);

    mDepthMapRenderTarget.BeginRender();
}

void ShadowEffect::End()
{
    mDepthMapRenderTarget.EndRender();
}

void ShadowEffect::Render(const RenderObject& renderobj)
{
    const auto& matWorld = renderobj.trasf.GetMatrix4();
    const auto& matView = mLightCam.GetViewMatrix();
    const auto& matProj = mLightCam.GetProjectionMatrix();

    TransformData transformData;
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mTransforBuffer.Update(transformData);

    renderobj.meshBuffer.Render();
}

void ShadowEffect::SetDirectionLight(const DirectionaLight& directionalLight)
{
    mDirectionalLight = &directionalLight;
}

void ShadowEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("ShadowEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Depth Map");
        ImGui::Image(
            mDepthMapRenderTarget.GetRawData(),
            { 144,144 },
            { 0,0 },
            { 1,1 },
            { 1,1,1,1 },
            { 1,1,1,1 }
        );
    }
    ImGui::DragFloat("Size", &mSize, 1.0f, 1.0f, 1000.0f);
}

void ShadowEffect::UpdateLightCam()
{
    ASSERT(mDirectionalLight != nullptr, "ShadowEffect -- no light here");

    const GGMath::Vector3& direction = mDirectionalLight->direction;
    mLightCam.SetDirection(direction);
    mLightCam.SetPosition(mFocusPosition - (direction * 1000.0f));
    mLightCam.SetNearPlane(1.0f);
    mLightCam.SetFarPlane(1500.0f);
    mLightCam.SetSize(mSize, mSize);
}
