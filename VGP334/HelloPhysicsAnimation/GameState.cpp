#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::Graphic::AnimationUtil;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,10.0f,-5.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mDirectionalLight.direction = Normalize({ 1.0f,-1.0f,1.0f });
    mDirectionalLight.ambient = { 0.8f,0.8f,0.8f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.sepcular = { 0.7f,0.7f,0.7f,1.0f };

    Mesh ball = MeshBuilder::CreateSphere(30, 30, 1.0f);
    mBall.meshBuffer.Initialize(ball);
    mBall.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/misc/basketball.jpg");
    mBall.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mBall.material.diffuse = { 0.5f,0.5f,0.5f,1.0f };
    mBall.material.specular = { 0.5f,0.5f,0.5f,1.0f };
    mBall.material.power = 10.0f;
    mBall.trasf.position.y = 8.0f;

    //RigidBody
    spherC.Initialize(1.0f);
    rb.Initialize(mBall.trasf,spherC, 5.0f);

    Mesh ground = MeshBuilder::CreatePlane(20, 20, 1.0f);
    mGround.meshBuffer.Initialize(ground);
    mGround.diffuesMapID = TextureManger::Get()->LoadTexture(L"Textures/misc/concrete.jpg");
    mGround.material.ambient = { 0.5f,0.5f,0.5f,1.0f };
    mGround.material.diffuse = { 0.5f,0.5f,0.5f,1.0f };
    mGround.material.specular = { 0.5f,0.5f,0.5f,1.0f };
    mGround.material.power = 10.0f;

    groundCP.Initialize({ 10.0f,0.05f,10.0f });
    rbGround.Initialize(mGround.trasf, groundCP);


    mStandardEffect.Initialize(L"../../Assets/Shaders/Standard.fx");
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
}

void GameState::Terminate()
{
    rb.Terminate();
    spherC.Terminate();
    mBall.Terminate();
    
    rbGround.Terminate();
    groundCP.Terminate();
    mGround.Terminate();
    
    mStandardEffect.Terminate();
}

int gAnimationIdx = 0;

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
        mStandardEffect.Render(mBall);
        mStandardEffect.Render(mGround);
    mStandardEffect.End();

    //SimpleDraw::AddGroundPlane(20.0f, Colors::Red);
    //SimpleDraw::Render(mCamera);
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
    mStandardEffect.DebugUI();
    PhysicsWorld::Get()->DebugUI();

    ImGui::End();
    SimpleDraw::Render(mCamera);
}