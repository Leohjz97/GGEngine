#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    light.direction = Normalize({ 1.0f,-1.0f,1.0f });
    light.ambient = { 0.8f,0.8f,0.8f,1.0f };
    light.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    light.sepcular = { 0.7f,0.7f,0.7f,1.0f };

    sphere = MeshBuilder::CreateSphere(320, 320, 1);

    mEarthObject.meshBuffer.Initialize(sphere);
    mEarthObject.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/earth_normal.jpg");
    mEarthObject.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/earth.jpg");
    mEarthObject.normalMapID = TextureManger::Get()->LoadTexture(L"Textures/earth_normal.jpg");
    mEarthObject.displacementMapID = TextureManger::Get()->LoadTexture(L"Textures/earth_bump.jpg");
    mEarthObject.specularMapID = TextureManger::Get()->LoadTexture(L"Textures/earth_spec.jpg");

    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(light);
}

void GameState::Terminate()
{
    mStandardEffect.Terminate();
    mEarthObject.Terminate();
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
    mStandardEffect.Begin();
    mStandardEffect.Render(mEarthObject);
    mStandardEffect.End();

    ShowNormal();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug Control", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Light",ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction##Light", &light.direction.x,0.0f,-1.0f,1.0f))
        {
            light.direction = Normalize(light.direction);
        }

        ImGui::ColorEdit4("Ambient##Light", &light.ambient.r);
        ImGui::ColorEdit4("Diffuse##Light", &light.diffuse.r);
        ImGui::ColorEdit4("Specular##Light", &light.sepcular.r);
    }

    ImGui::Separator();
    ImGui::Checkbox("Draw Normals", &mShowNormal);
    mStandardEffect.DebugUI();
    ImGui::End();
}

void GameState::ShowNormal()
{
    if (mShowNormal)
    {
        for (const auto& v :sphere.verticies)
        {
            SimpleDraw::AddLine(v.position, v.position + (v.normal*0.2f), Colors::Red);
        }

        SimpleDraw::Render(mCamera);
    }
}