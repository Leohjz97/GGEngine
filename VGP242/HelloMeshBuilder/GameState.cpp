#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

void GameState::OnCreateShape()
{
    mVerticies.push_back({ Vector3(0.0f,0.5f,0.0f),Colors::Blue });
    mVerticies.push_back({ Vector3(0.5f,-0.5f,0.0f),Colors::Red });
    mVerticies.push_back({ Vector3(-0.5f,-0.5f,0.0f),Colors::Yellow });

    mVerticies.push_back({ Vector3(1.0f,0.5f,0.0f),Colors::Red });

    mVerticies.push_back({ Vector3(1.0f,0.5f,0.0f),Colors::Yellow });

    std::vector<int> indicies;
    indicies.push_back(0);
    indicies.push_back(1);
    indicies.push_back(2);

    indicies.push_back(2);
    indicies.push_back(3);
    indicies.push_back(0);

    indicies.push_back(0);
    indicies.push_back(4);
    indicies.push_back(1);

}
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
    mesh = MeshBuilder::CreateSphere(36, 36, 24);

    //float -> 4 bytes
    //4*3*9=108 bytes

    mConstantBuffer.Initialize(sizeof(Matrix4));
    mMeshBuffer.Initialize(mesh);
    mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx", VE_Position | VE_Color);
    //mVertexShader.Initialize("../../Assets/Shaders/DoTransform.fx");
    mPiexlShader.Initialize("../../Assets/Shaders/DoTransform.fx");
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
    
    mMeshBuffer.Render();
}

void GameState::CreateCube()
{
}

//void DebugUI()
//{
//
//}