#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f,1.0f,-3.0f });
    mCamera.SetLookAt({ 0.0f,0.0f,0.0f });
}

void GameState::Terminate()
{
}

void GameState::Update(float deltTime)
{
}

void GameState::Render()
{
    if (mIsLine)
    {
        SimpleDraw::AddLine(mLinesP1, mLinesP2, mLineColor);
    }
    else
    {
        SimpleDraw::AddSphere(mSlice, mRings, mCircleRad, mCircleColor);
    }
    SimpleDraw::Render(mCamera);
}
void GameState::DebugUI()
{
    ImGui::Begin("Debug Draw", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Checkbox("IsLine", &mIsLine);
    if (mIsLine)
    {
        ImGui::Begin("line data", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::DragFloat("Pos1 X", &mLinesP1.x, 0.01f);
        ImGui::DragFloat("Pos2 X", &mLinesP2.x, 0.01f);
        ImGui::ColorEdit4("Color", &mLineColor.r);
        ImGui::End();
    }
    else
    {
        ImGui::Begin("Sphere Data", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
        ImGui::DragInt("Slice", &mSlice, 0.01f);
        ImGui::DragInt("Ring", &mRings, 0.01f);
        ImGui::DragFloat("Radius", &mCircleRad, 0.01f);
        ImGui::ColorEdit4("Color", &mCircleColor.r);
        ImGui::End();
    }
    ImGui::End();
}