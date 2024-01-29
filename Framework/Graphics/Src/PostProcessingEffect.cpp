#include "Precompiled.h"
#include "PostProcessingEffect.h"
#include "Camera.h"
#include "GraphicSystem.h"
#include "RenderObject.h"
#include "VertexTypes.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void PostProcessingEffect::Intitialize(const std::filesystem::path& filePath)
{
    mVertexShader.Initialize<VertexPX>(filePath);
    mPixelShader.Initialize(filePath);

    mPostProcessBuffer.Initialize();

    mSampler.Initialize(Samlper::Filter::Point, Samlper::AddressMode::Wrap);
}

void PostProcessingEffect::Terminate()
{
    mSampler.Terminate();

    mPostProcessBuffer.Terminate();
    
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}

void PostProcessingEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();

    PostProcessData data;
    data.mode = mMode;
    switch (mMode)
    {
    case Mode::None:
        {
        break;
        }
    case Mode::Invert:
    {
        break;
    }
    case Mode::Mirror:
    {
        data.param0 = mMirrorScale;
        break;
    }
    case Mode::Blur:
    {
        auto gs = GraphicSystem::Get();
        const auto screenWid = gs->GetBackBufferWidth();
        const auto screenHei = gs->GetBackBufferHeight();
        data.param0 = 5.0f / screenWid;
        data.param1 = 5.0f / screenHei;
        break;
    }
    }

    mPostProcessBuffer.Update(data);
    mPostProcessBuffer.BindPS(0);

    mSampler.BindPS(0);

    for (int i = 0; i < mTextures.size(); ++i)
    {
        if (mTextures[i] != nullptr)
        {
            mTextures[i]->BindPS(i);
        }
    }
}

void PostProcessingEffect::End()
{
    for (int i = 0; i < mTextures.size(); ++i)
    {
        Texture::UnbindPS(i);
    }
}

void PostProcessingEffect::Render(const RenderObject& obj)
{
    obj.meshBuffer.Render();
}

void PostProcessingEffect::SetTexture(const Texture* tex, uint32_t slot)
{
    ASSERT(slot < mTextures.size(), "PostProcessingEffect -- invalid slot index");
    mTextures[slot] = tex;
}

void PostProcessingEffect::SetMode(Mode mode)
{
    mMode = mode;
}

void PostProcessingEffect::SetMirrorScale(float mirrorScale)
{
    mMirrorScale = mirrorScale;
}

void PostProcessingEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("PostProcessingEffect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        int currentMode = static_cast<int>(mMode);
        const char* modeNames[] =
        {
            "None",
            "Monochrome",
            "Invert",
            "Mirror",
            "Blur",
            "Combine2"
        };

        if (ImGui::Combo("Mode", &currentMode,modeNames,static_cast<int>(std::size(modeNames))))
        {
            mMode = static_cast<Mode>(currentMode);
        }

        if (mMode == Mode::Mirror)
        {
            ImGui::DragFloat("Scale", &mMirrorScale, 0.1f, -0.5f, 0.5f);
        }
    }
}
