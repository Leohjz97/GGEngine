#include"Precompiled.h"
#include"StandardEffect.h"
#include"Camera.h"
#include"RenderObject.h"
#include"VertexTypes.h"
#include"AnimationUtil.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

static constexpr size_t MaxBoneCount = 256;


void StandardEffect::Initialize(const std::filesystem::path& filePath)
{
    mTransferbuffer.Initialize();
    mLightBuffer.Initialize();
    mBoneTransferbuffer.Initialize(MaxBoneCount*sizeof(GGMath::Matrix4));

    mMatericalBuffer.Initialize();
    mSettingsBuffer.Initialize();
    mVertexShader.Initialize<Vertex>(filePath);
    mPiexlShader.Initialize(filePath);
    mSamlper.Initialize(Samlper::Filter::Linear, Samlper::AddressMode::Wrap);
}

void StandardEffect::Terminate()
{
    mSamlper.Terminate();
    mPiexlShader.Terminate();
    mMatericalBuffer.Terminate();
    mSettingsBuffer.Terminate();
    mVertexShader.Terminate();
    mLightBuffer.Terminate();
    mTransferbuffer.Terminate();
    mBoneTransferbuffer.Terminate();
}

void StandardEffect::Begin()
{
    ASSERT(mCamera != nullptr, "StandardEffect - no camera has been set!");
    ASSERT(mLight != nullptr, "StandardEffect - no light has been set!"); 
    mVertexShader.Bind();
    mPiexlShader.Bind();

    mTransferbuffer.BindVS(0);

    mBoneTransferbuffer.BindVS(1);

    mLightBuffer.BindPS(2);
    mLightBuffer.BindVS(2);

    mMatericalBuffer.BindVS(3);
    mMatericalBuffer.BindPS(3);

    mSettingsBuffer.BindPS(4);
    mSettingsBuffer.BindVS(4);

    mSamlper.BindPS(0);
    mSamlper.BindVS(0);
}

void StandardEffect::End()
{
    if (mShadowMap != nullptr)
    {
        Texture::UnbindPS(4);
    }
}

void StandardEffect::Render(const RenderObject& renderobject)
{
    const auto& matWorld = renderobject.trasf.GetMatrix4();
    const auto& matView = mCamera->GetViewMatrix();
    const auto& matProj = mCamera->GetProjectionMatrix();


    SettingsData settingsData;
    settingsData.useDiffuseMap = mSettingsData.useDiffuseMap > 0 && renderobject.diffuesMapID > 0;
    settingsData.useDisplacementMap = mSettingsData.useDisplacementMap > 0 && renderobject.displacementMapID > 0;
    settingsData.useNormalMap= mSettingsData.useNormalMap > 0 && renderobject.normalMapID > 0;
    settingsData.useSpecularMap= mSettingsData.useSpecularMap > 0 && renderobject.specularMapID > 0;
    settingsData.useShadowMap=mSettingsData.useShadowMap > 0 && mShadowMap != nullptr;
    settingsData.useSkinning = mSettingsData.useSkinning > 0 && renderobject.skeleton != nullptr;
    settingsData.depthBias = mDepthBias;

    TransformData trafData;
    trafData.world = Transpose(matWorld);
    trafData.wvp[0] = Transpose(matWorld * matView * matProj);
    trafData.viewPostion = mCamera->GetPosition();
    trafData.displacementWeight = mDisplacementWeight;
    if (settingsData.useShadowMap)
    {
        const auto& matLightView = mLightCamera->GetViewMatrix();
        const auto& matLightProj = mLightCamera->GetProjectionMatrix();
        trafData.wvp[1] = Transpose(matWorld * matLightView * matLightProj);

        mShadowMap->BindPS(4);
    }
    
    if (settingsData.useSkinning)
    {
        AnimationUtil::BoneTransforms boneTransforms;
        AnimationUtil::ComputeBoneTransform(renderobject.modelid, boneTransforms,renderobject.animator);
        AnimationUtil::ApplyBoneOffsets(renderobject.modelid, boneTransforms);

        for (auto& transform : boneTransforms)
        {
            transform = Transpose(transform);
        }

        boneTransforms.resize(MaxBoneCount);
        mBoneTransferbuffer.Update(boneTransforms.data());
    }

    mTransferbuffer.Update(trafData);
    mLightBuffer.Update(*mLight);
    mMatericalBuffer.Update(renderobject.material);
    mSettingsBuffer.Update(settingsData);

    //Send data to shader and display
    auto tm = TextureManger::Get();
    tm->BindPS(renderobject.diffuesMapID, 0);
    tm->BindPS(renderobject.normalMapID, 1);
    tm->BindVS(renderobject.displacementMapID, 2);
    tm->BindPS(renderobject.specularMapID, 3);
    renderobject.meshBuffer.Render();
}

void StandardEffect::SetCamera(const Camera& cam)
{
    mCamera =& cam;
}

void StandardEffect::SetDirectionalLight(const DirectionaLight& light)
{
    mLight =& light;
}

void StandardEffect::SetLightCamerea(const Camera& camera)
{
    mLightCamera = &camera;
}

void StandardEffect::SetShadowMap(const Texture* shadowMap)
{
    mShadowMap = shadowMap;
}

void StandardEffect::DebugUI()
{
    if (ImGui::CollapsingHeader("Standard Effect", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Displacement Weight", &mDisplacementWeight, 0.1f);
        
        bool useDiffuseMap = mSettingsData.useDiffuseMap > 0;
        ImGui::Checkbox("Use Diffuse Map", &useDiffuseMap);
        mSettingsData.useDiffuseMap = (useDiffuseMap) ? 1 : 0;

        bool useSpeculaMap = mSettingsData.useSpecularMap > 0;
        ImGui::Checkbox("Use Specular Map", &useSpeculaMap);
        mSettingsData.useSpecularMap = (useSpeculaMap) ? 1 : 0;
        
        bool useDisplacementMap = mSettingsData.useDisplacementMap > 0;
        ImGui::Checkbox("use Displacement Map", &useDisplacementMap);
        mSettingsData.useDisplacementMap = (useDisplacementMap) ? 1 : 0;

        bool useNormalMap = mSettingsData.useNormalMap > 0;
        ImGui::Checkbox("use Normal Map", &useNormalMap);
        mSettingsData.useNormalMap = (useNormalMap) ? 1 : 0;

        bool useShadowMap = mSettingsData.useShadowMap > 0;
        ImGui::Checkbox("use Shadow Map", &useShadowMap);
        mSettingsData.useShadowMap = (useShadowMap) ? 1 : 0;

        ImGui::DragFloat("Depth Bias", &mDepthBias, 0.0000001f, 0.0f, 1.0f, "%.6f");

    }
}