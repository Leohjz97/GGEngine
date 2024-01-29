#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-4.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.sepcular = { 0.7f,0.7f,0.7f,1.0f };

    Mesh ground = MeshBuilder::CreatePlane(20, 20, 1.0f);
    mPlane.meshBuffer.Initialize(ground);
    mPlane.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/earth.jpg");
    mPlane.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mPlane.material.diffuse = { 0.5f,0.5f,0.5f,1.0f };
    mPlane.material.specular = { 0.5f,0.5f,0.5f,1.0f };
    mPlane.material.power = 10.0f;

    Model character;
    ModelIO::LoadModel("../../Assets/Models/Ortiz/Ortiz.model", character);
    ModelIO::LoadMaterial("../../Assets/Models/Ortiz/Ortiz.material", character);
    mModel = CreateRenderGroup(character);


    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamerea(mShadowEffect.GetLightCam());
    mStandardEffect.SetShadowMap(&mShadowEffect.GetDepthMap());
    
    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
    mPlane.Terminate();
    CleanupRenderGroup(mModel);
}

void GameState::Update(float deltTime)
{
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

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltTime);
    }
}

Vector3 camerPos = Vector3::Zero;
void GameState::Render()
{
    mShadowEffect.Begin();
        DrawRenderGroup(mShadowEffect, mModel);
    mShadowEffect.End();

    mStandardEffect.Begin();
        DrawRenderGroup(mStandardEffect, mModel);
        mStandardEffect.Render(mPlane);
    mStandardEffect.End();
    //ShowNormal();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
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

    ImGui::Separator();
    mStandardEffect.DebugUI();
    mShadowEffect.DebugUI();

    ImGui::End();
}