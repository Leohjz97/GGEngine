#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-10.0f });
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

    Mesh sphere = MeshBuilder::CreateSphere(60, 60, 1.0f);
    mSphere.meshBuffer.Initialize(sphere);
    mSphere.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/misc/basketball.jpg");
    mSphere.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mSphere.material.diffuse = { 0.5f,0.5f,0.5f,1.0f };
    mSphere.material.specular = { 0.5f,0.5f,0.5f,1.0f };
    mSphere.material.power = 10.0f;
    mSphere.trasf.position.y = 1.0f;

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    mPlane.Terminate();
    mSphere.Terminate();
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

    //mYaw += 1.0f*deltTime;
    mSphere.trasf.rotation = Quaternion::CreateFromYawPitchRoll(mYaw, mPitch, mRoll);
}

Vector3 camerPos = Vector3::Zero;
void GameState::Render()
{
    mStandardEffect.Begin();
        mStandardEffect.Render(mPlane);
        mStandardEffect.Render(mSphere);
    mStandardEffect.End();
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

    if (ImGui::CollapsingHeader("Quaternion", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Yaw##", &mYaw, 0.1f);
        ImGui::DragFloat("Pitch##", &mPitch, 0.1f);
        ImGui::DragFloat("Roll##", &mRoll, 0.1f);
    }


    ImGui::Separator();
    mStandardEffect.DebugUI();

    ImGui::End();
}