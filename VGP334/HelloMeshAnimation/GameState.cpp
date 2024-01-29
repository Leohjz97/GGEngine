#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::Graphic::AnimationUtil;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.sepcular = { 0.7f,0.7f,0.7f,1.0f };

    Mesh ground = MeshBuilder::CreatePlane(20, 20, 1.0f);
    mPlane.meshBuffer.Initialize(ground);
    mPlane.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/misc/concrete.jpg");
    mPlane.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mPlane.material.diffuse = { 0.5f,0.5f,0.5f,1.0f };
    mPlane.material.specular = { 0.5f,0.5f,0.5f,1.0f };
    mPlane.material.power = 10.0f;


    auto mm = ModelManager::Get();
    mCharacterModelid = mm->LoadModelID(L"../../Assets/Models/Doozy/ThrillerPart4.model");
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Action/walking.animset");
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Action/running.animset");
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Action/jumpingup.animset");
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Action/runtostop.animset");
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Action/standtocover.animset");
    
    mCharacterAnimator.Initialize(mCharacterModelid);
    mCharacter = CreateRenderGroup(mCharacterModelid, &mCharacterAnimator);

    mCharacterAnimator.PlayAnimation(2, true);

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mDrawSkeleton = false;
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    mPlane.Terminate();
    CleanupRenderGroup(mCharacter);
}

int gAnimationIdx = 0;

void GameState::Update(float deltTime)
{
    mCharacterAnimator.Update(deltTime);

    //movement
    auto input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltTime);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltTime);
    }
    
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltTime);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltTime);
    }

    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltTime);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltTime);
    }

    if (input->IsKeyPressed(KeyCode::UP))
    {
        size_t animCount = mCharacterAnimator.GetAnimationCount();
        gAnimationIdx = (gAnimationIdx + 1) % animCount;
        mCharacterAnimator.PlayAnimation(gAnimationIdx, true,2.0f);
    }
    else if (input->IsKeyPressed(KeyCode::DOWN))
    {
        size_t animCount = mCharacterAnimator.GetAnimationCount();
        gAnimationIdx = (gAnimationIdx - 1);
        if (gAnimationIdx < 0)
        {
            gAnimationIdx = animCount - 1;
        }
        mCharacterAnimator.PlayAnimation(gAnimationIdx, true);
    }
        
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltTime);
    }

}

Vector3 camerPos = Vector3::Zero;
void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mPlane);
        if (mDrawSkeleton)
        {
            BoneTransforms boneTransforms;
            ComputeBoneTransform(mCharacterModelid, boneTransforms,&mCharacterAnimator);
            DrawSkeleton(mCharacterModelid, boneTransforms);

            SimpleDraw::Render(mCamera);
        }
        else
        {
            DrawRenderGroup(mStandardEffect, mCharacter);
        }
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Light",ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x,0.0f,-1.0f,1.0f))
        {
            mDirectionalLight.direction = Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &mDirectionalLight.sepcular.r);
    }
    //ImGui::Separator();
    ImGui::Checkbox("Draw##Skeleton", &mDrawSkeleton);
    mStandardEffect.DebugUI();
    ImGui::End();
}