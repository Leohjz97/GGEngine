#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

void GameState::Initialize()
{
    // NDC - Normalized Device Coordinate
    //
    //            +-----------------+
    //           /                 /|
    //     1.0  +-----------------+ |
    //          |        ^        | |
    //          |        |        | |
    //          | <------+------> | |
    //          |        |        | | 1.0
    //          |        v        |/
    //    -1.0  +-----------------+ 0.0
    //      -1.0               1.0
    //
    mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    //mesh = MeshBuilder::CreateCubePC(1.0f, Colors::CadetBlue);
    //mesh = MeshBuilder::CreateRectanglePC(4, 4, 4);
    //mesh = MeshBuilder::CreatePlanePC(1, 1, 2);
    //mesh = MeshBuilder::CreateCylinderPC(24, 3);
    //mesh = MeshBuilder::CreateSphere(48, 48, 36);
    sphere = MeshBuilder::CreateSpherePX(60, 60, 1);

    mConstantBuffer.Initialize(sizeof(Matrix4));
    mMeshBuffer.Initialize(sphere);
    mEarthID = TextureManger::Get()->LoadTexture(L"Textures/earth.jpg");
    mVertexShader.Initialize(L"../../Assets/Textures/DoTexturing.fx", VE_Position | VE_TexCoord);
    mPiexlShader.Initialize(L"../../Assets/Textures/DoTexturing.fx");
}

void GameState::Terminate()
{
    mConstantBuffer.Terminate();
    mMeshBuffer.Terminate();
    mVertexShader.Terminate();
    mPiexlShader.Terminate();
}

void GameState::Update(float deltTime)
{
    //movement
    //physics
    //trigger
    //result goes to rendering
    //move the camera position
    auto inputSystem = Input::InputSystem::Get();
    if (inputSystem->IsKeyDown(Input::KeyCode::UP) ||
        inputSystem->IsKeyPressed(Input::KeyCode::W))
    {
        Xspeed += 5.0f * deltTime;
    }
    if (inputSystem->IsKeyDown(Input::KeyCode::DOWN) ||
        inputSystem->IsKeyPressed(Input::KeyCode::S))
    {
        Xspeed -= 5.0f * deltTime;
    }
    if (inputSystem->IsKeyDown(Input::KeyCode::LEFT) ||
        inputSystem->IsKeyPressed(Input::KeyCode::A))
    {
        Yspeed -= 5.0f * deltTime;
    }
    if (inputSystem->IsKeyDown(Input::KeyCode::RIGHT) ||
        inputSystem->IsKeyPressed(Input::KeyCode::D))
    {
        Yspeed += 5.0f * deltTime;
    }
}

static float rotation = 0.0f;
void GameState::Render()
{
    mVertexShader.Bind();
    mPiexlShader.Bind();

    //rotation += (0.01f);

    Matrix4 cubeWorldTransX = Matrix4::RotationX(Xspeed);
    Matrix4 cubeWorldTransY = Matrix4::RotationY(Yspeed);
    Matrix4 view = mCamera.GetViewMatrix();
    Matrix4 proj = mCamera.GetProjectionMatrix();
    Matrix4 wvp = Transpose(cubeWorldTransX * cubeWorldTransY * view * proj);
    mConstantBuffer.Update(&wvp);
    mConstantBuffer.BindVS(0);
    
    TextureManger::Get()->BindVS(mEarthID, 0);
    TextureManger::Get()->BindPS(mEarthID, 0);

    mMeshBuffer.Render();
}
//void DebugUI()
//{
//
//}