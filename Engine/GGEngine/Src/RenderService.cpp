#include "Precompiled.h"
#include "RenderService.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "MeshComponent.h"

#include "CameraService.h"
#include "GameWorld.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;

void RenderService::Initialize()
{
    mCameraService = GetWorld().GetService<CameraService>();
    
    mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.diffuse = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.sepcular = { 0.9f,0.9f,0.9f,1.0f };

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    
    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionLight(mDirectionalLight);
}

void RenderService::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
}

void RenderService::Update(float deltaTime)
{
    mFPS = 1.0 / deltaTime;
}

void RenderService::Render()
{
    const auto& camera = mCameraService->GetMain();
    mStandardEffect.SetCamera(camera);

    for (auto& entry : mRenderEntities)
    {
        for (auto& renderObject : entry.renderGroup )
        {
            renderObject.trasf = *entry.transforComponent;
        }
    }

    mShadowEffect.Begin();
    for (auto& entry : mRenderEntities)
    {
        DrawRenderGroup(mShadowEffect, entry.renderGroup);
    }
    mShadowEffect.End();

    mStandardEffect.Begin();
    for (auto& entry : mRenderEntities)
    {
        DrawRenderGroup(mStandardEffect, entry.renderGroup);
    }
    mStandardEffect.End();
}

void RenderService::DebugUI()
{
    ImGui::Text("FPS: %f", mFPS);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.0f, -1.0f, 1.0f))
        {
            mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.sepcular.r);
    }

    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();

    SimpleDraw::Render(mCameraService->GetMain());
}

void RenderService::SetDirectionalLight(const GGMath::Vector3& direction)
{
    mDirectionalLight.direction = GGEngine::GGMath::Normalize(direction);
}

void RenderService::Register(const ModelComponent* modelComponent)
{
    auto& entry = mRenderEntities.emplace_back();
    
    auto& gameObject = modelComponent->GetOwner();
    entry.modelComponent = modelComponent;
    entry.transforComponent = gameObject.GetComponent<TransformComponent>();

    const Animator* animator = nullptr;
    auto animatorComponent = gameObject.GetComponent<AnimatorComponent>();
    if (animatorComponent != nullptr)
    {
        animator = &animatorComponent->GetAnimator();
    }

    entry.renderGroup = CreateRenderGroup(modelComponent->GetModelID(), animator);

}

void RenderService::UnRegister(const ModelComponent* modelComponent)
{
    auto iter = std::find_if(
        mRenderEntities.begin(),
        mRenderEntities.end(),
        [&](const Entry& entry)
        {
            return entry.modelComponent == modelComponent;
        });

    if (iter != mRenderEntities.end())
    {
        auto& entry = *iter; CleanupRenderGroup(entry.renderGroup);
        mRenderEntities.erase(iter);
    }
}

void GGEngine::RenderService::Register(const MeshComponent* meshComponent)
{
    auto& entry = mRenderEntities.emplace_back();

    auto& gameObject = meshComponent->GetOwner();
    entry.meshComponent = meshComponent;
    entry.transforComponent = gameObject.GetComponent<TransformComponent>();
    entry.renderGroup.push_back(meshComponent->GetRenderObject());
}

void GGEngine::RenderService::UnRegister(const MeshComponent* meshComponent)
{
    auto iter = std::find_if(
        mRenderEntities.begin(),
        mRenderEntities.end(),
        [&](const Entry& entry)
        {
            return entry.meshComponent == meshComponent;
        });

    if (iter != mRenderEntities.end())
    {
        auto& entry = *iter; CleanupRenderGroup(entry.renderGroup);
        mRenderEntities.erase(iter);
    }
}
