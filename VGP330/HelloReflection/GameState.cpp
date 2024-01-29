#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

namespace
{
    Camera RenderReflection(const Camera& camera, float height)
    {
        Vector3 reflectLookat, reflectDirection, reflectPosition;

        reflectLookat.x = 0.0f;
        reflectLookat.y = 1.0f;
        reflectLookat.z = 0.0f;

        reflectPosition.x = camera.GetPosition().x;
        //reflectPosition.y = -camera.GetPosition().y + (height * 2.0f);
        reflectPosition.y = -camera.GetPosition().y + height;
        reflectPosition.z = camera.GetPosition().z;

        reflectDirection.x = camera.GetDirection().x;
        reflectDirection.y = -camera.GetDirection().y;
        reflectDirection.z = camera.GetDirection().z;

        Camera reflectionCamera;
        reflectionCamera.SetLookAt(reflectLookat);
        reflectionCamera.SetDirection(reflectDirection);
        reflectionCamera.SetPosition(reflectPosition);

        return reflectionCamera;
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,20.0f,-15.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });

    mReflectionCamera = RenderReflection(mCamera, mHeight);
    
    mDirectionalLight.direction = Normalize({ 1.0f, -1.0f, 1.0f });
    mDirectionalLight.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
    mDirectionalLight.diffuse = { 0.7f, 0.7f, 0.7f, 1.0f };
    mDirectionalLight.sepcular = { 0.7f, 0.7f, 0.7f, 1.0f };

    //mMaterial.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
    //mMaterial.diffuse = { 0.85f, 0.85f, 0.85f, 1.0f };
    //mMaterial.specular = { 0.5f, 0.5f, 0.5f, 1.0f };
    //mMaterial.power = 10.0f;

    mPower = 0.15f;
    const auto gs = GraphicSystem::Get();
    const auto screenHeight = gs->GetBackBufferHeight();
    const auto screenWidth = gs->GetBackBufferWidth();
    mReflectRenderTarget.Initialize(screenWidth, screenHeight, Texture::Format::RGBA_U32);

    Mesh mPlane = MeshBuilder::CreatePlane(50, 50, 1.0f);
    mGroundMB.Initialize(mPlane);
    mGroundVS.Initialize(L"../../Assets/Shaders/ReflectionEffect.fx", Vertex::Format);
    mGroundPS.Initialize(L"../../Assets/Shaders/ReflectionEffect.fx");
    //mGroundTexture.Initialize("../../Assets/Textures/white.png");
    mGroundTexture.Initialize("../../Assets/Textures/black.jpg");
    
    Mesh sphere = MeshBuilder::CreateSphere(320.0f, 320.0f, 2.0f);
    mSphereMB.Initialize(sphere);
    mSphereVS.Initialize(L"../../Assets/Shaders/DoTexturing.fx", Vertex::Format);
    mSpherePS.Initialize(L"../../Assets/Shaders/DoTexturing.fx");
    mSphereTexture.Initialize("../../Assets/Textures/Moon.jpg");
    //mSphereTexture.Initialize("../../Assets/Textures/Moon2.jpg");
    
    mSampler.Initialize(Samlper::Filter::Linear, Samlper::AddressMode::Clamp);
    mPowerBuffer.Initialize();
    mReflectionBuffer.Initialize();
    mReflectionTransformBuffer.Initialize();
    mTransformBuffer.Initialize();

}

void GameState::Terminate()
{
    mReflectRenderTarget.Terminate();

    mGroundMB.Terminate();
    mGroundVS.Terminate();
    mGroundPS.Terminate();
    mGroundTexture.Terminate();

    mSphereMB.Terminate();
    mSphereVS.Terminate();
    mSpherePS.Terminate();
    mSphereTexture.Terminate();

    mSampler.Terminate();
    mPowerBuffer.Terminate();
    mReflectionBuffer.Terminate();
    mReflectionTransformBuffer.Terminate();
    mTransformBuffer.Terminate();
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

static float rotation = 0.0f;
void GameState::Render()
{
    Matrix4 matView = mCamera.GetViewMatrix();
    Matrix4 matProj = mCamera.GetProjectionMatrix();

    rotation += 0.01f;
    Matrix4 matWorld = Matrix4::RotationY(rotation) * Matrix4::Translation({ 0.0f, 20.0f + mHeight, 0.0f });

    TransformData transformData;
    transformData.wvp = Transpose(matWorld * matView * matProj);
    mTransformBuffer.Update(transformData);
    mTransformBuffer.BindVS(0);

    mSphereVS.Bind();
    mSpherePS.Bind();
    mSphereTexture.BindPS(0);
    mSampler.BindPS(0);
    mSphereMB.Render();

    mReflectionCamera = RenderReflection(mCamera, mHeight);
    ReflectionTransformData reflectionTrasfData;
    ReflectionBufferData reflectionBufferData;
    matProj = mReflectionCamera.GetProjectionMatrix();
	matWorld = Matrix4::Translation({ 0.0f, 0.0f, 0.0f }) * Matrix4::RotationX(mRotation.x) * Matrix4::RotationY(mRotation.y);
    reflectionTrasfData.world = Transpose(matWorld);
    reflectionTrasfData.view = Transpose(matView);
    reflectionTrasfData.porj = Transpose(matProj);

    mReflectionTransformBuffer.Update(reflectionTrasfData);
    mReflectionTransformBuffer.BindVS(0);
    
    Matrix4 matReflection = mReflectionCamera.GetViewMatrix();
    reflectionBufferData.reflectionM = Transpose(matReflection);
    mReflectionBuffer.Update(reflectionBufferData);
    mReflectionBuffer.BindVS(1);
    mReflectRenderTarget.BindPS(1);

    PowerBufferData powerBufferData;
    powerBufferData.power = mPower;
    mPowerBuffer.BindVS(2);
    mPowerBuffer.BindPS(2);
    mPowerBuffer.Update(powerBufferData);

    mGroundVS.Bind();
    mGroundPS.Bind();
    mGroundTexture.BindPS(0);
    mGroundMB.Render();

    mReflectRenderTarget.BeginRender();
        mTransformBuffer.BindVS(0);
        mSphereVS.Bind();
        mSpherePS.Bind();
        mSphereTexture.BindPS(0);
        mSampler.BindPS(0);
        mSphereMB.Render();
    mReflectRenderTarget.EndRender();
}

void GameState::DebugUI()
{
    ImGui::Begin("Reflection", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Height", &mHeight, 0.01f);
        ImGui::DragFloat("Power", &mPower, 0.01f, 0.0f, 0.5f);
    }
    ImGui::End();
}