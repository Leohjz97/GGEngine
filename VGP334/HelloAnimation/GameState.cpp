#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-20.0f });
    mCamera.SetLookAt({ 0.0f,5.0f,0.0f });
    mCamera.SetMode(Camera::ProjectionMode::Perspective);

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

    animationTime = 0.0f;
    mSphereAnimation = AnimationBuilder()
        .AddPositionKey(Vector3(-3.0f, 1.0f, 0.0f), 0.0f, EaseInOutCubic)
        .AddPositionKey(Vector3(0.0f, 4.0f, 0.0f), 2.0f, EaseInOutBack)
        .AddScaleKey(Vector3(2.0f, 2.0f, 0.0f), 2.0f, Linear)
        .AddPositionKey(Vector3(1.0f, 7.0f, 2.0f), 4.0f, EaseInOutElastic)
        .AddPositionKey(Vector3(2.0f, 5.0f, 0.0f), 6.0f, EaseInBounce)
        .AddScaleKey(Vector3(0.0f, 0.0f, 0.0f), 6.0f, Linear)
        .AddPositionKey(Vector3(3.0f, 3.0f, 0.0f), 8.0f, EaseOutBounce)
        .AddPositionKey(Vector3(4.0f, 1.0f, 0.0f), 10.0f, EaseInOutBounce)
        .AddPositionKey(Vector3(-3.0f, 1.0f, 0.0f), 12.0f, EaseInOutQuad)
        .AddPositionKey(Vector3(0.0f, 3.0f, 0.0f), 14.0f, EaseInBack)
        .AddPositionKey(Vector3(-3.0f, 1.0f, 0.0f), 15.0f, EaseInElastic)
        .Build();
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

    //Animation
    animationTime += deltTime;
    const float animationDuration = mSphereAnimation.GetDuration();
    while (animationTime > animationDuration)
    {
        animationTime -= animationDuration;
    }
    mSphere.trasf = mSphereAnimation.GetTransform(animationTime);
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
    ImGui::Separator();
    mStandardEffect.DebugUI();

    ImGui::End();
}