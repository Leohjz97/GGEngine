#include "GameState.h"
#include "Input/Inc/InputSystem.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "ImGui/Src/imgui_internal.h"

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
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Behavior/Idle.animset");         //2
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Behavior/Walking.animset");      //3
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Behavior/Running.animset");      //4
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Behavior/Jumping.animset");      //5
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Behavior/RightStrafe.animset");  //6
    mm->AddAnimation(mCharacterModelid, L"../../Assets/Models/Doozy/Behavior/LeftStrafe.animset");   //7
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

int gAnimationIdx = 2;
bool isKeyPressed = false;

void GameState::Update(float deltTime)
{
    mCharacterAnimator.Update(deltTime);

    //movement
    auto input = InputSystem::Get();
    //const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

}

Vector3 camerPos = Vector3::Zero;
void GameState::Render()
{
    mStandardEffect.Begin();
    mStandardEffect.Render(mPlane);
    if (mDrawSkeleton)
    {
        BoneTransforms boneTransforms;
        ComputeBoneTransform(mCharacterModelid, boneTransforms, &mCharacterAnimator);
        DrawSkeleton(mCharacterModelid, boneTransforms);

        SimpleDraw::Render(mCamera);
    }
    else
    {
        DrawRenderGroup(mStandardEffect, mCharacter);
    }
    mStandardEffect.End();
}

bool IsAnimationWindowOPen = false;
float mouseX = 0.0f;
float mouseY = 0.0f;
void GameState::DebugUI()
{
    ImGui::Begin("Debug Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Separator();

    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Direction##Light", &mDirectionalLight.direction.x, 0.0f, -1.0f, 1.0f))
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

    ImGui::SetNextWindowSize(ImVec2(300, 400));
    ImGui::Checkbox("Animation Blend Machine", &IsAnimationWindowOPen);
    if (IsAnimationWindowOPen)
    {
        ImGui::Begin("Animation Blend State", nullptr, ImGuiWindowFlags_AlwaysAutoResize );
        //ImGui::Begin("Animation Blend State");

        ImGui::Separator();
        ImVec2 windowSize = ImGui::GetContentRegionAvail();

        float cellSize = windowSize.x / 20;

        ImVec2 planeCenter = ImGui::GetCursorScreenPos() + ImVec2(windowSize.x / 2, windowSize.y / 2);
        float planeSize = cellSize * 10;

        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawList->AddLine(planeCenter + ImVec2(-planeSize, 0), planeCenter + ImVec2(planeSize, 0), IM_COL32(255, 255, 255, 255));
        drawList->AddLine(planeCenter + ImVec2(0, -planeSize), planeCenter + ImVec2(0, planeSize), IM_COL32(255, 255, 255, 255));

        ImVec2 pointIdle = planeCenter + ImVec2(0,0); //Center
        ImVec2 pointWalk = planeCenter + ImVec2(0, -planeSize); //top
        ImVec2 pointJump = planeCenter + ImVec2(0, planeSize); //buttom
        ImVec2 pointRightStrafe = planeCenter + ImVec2(planeSize, 0);
        ImVec2 pointLeftStrafe = planeCenter + ImVec2(-planeSize, 0);

        drawList->AddCircleFilled(pointIdle, 5.0f, IM_COL32(255, 0, 0, 255));     //Red
        drawList->AddCircleFilled(pointWalk, 5.0f, IM_COL32(255, 255, 0, 255));   //Yellow
        drawList->AddCircleFilled(pointJump, 5.0f, IM_COL32(0, 255, 0, 255));         //Green
        drawList->AddCircleFilled(pointRightStrafe, 5.0f, IM_COL32(0, 0, 255, 255));    //Blue
        drawList->AddCircleFilled(pointLeftStrafe, 5.0f, IM_COL32(255, 0, 255, 255)); //Purple

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            ImVec2 mousePos = ImGui::GetMousePos();
            drawList->AddCircleFilled(mousePos, 5.0f, IM_COL32(255, 0, 255, 255)); //Purple
            if (ImGui::IsMouseHoveringRect(pointIdle - ImVec2(5, 5), pointIdle + ImVec2(5, 5)))
            {
                gAnimationIdx = 2;

                mCharacterAnimator.PlayAnimation(gAnimationIdx, true, 2.0f);
            }
            else if (ImGui::IsMouseHoveringRect(pointWalk - ImVec2(5, 5), pointWalk + ImVec2(5, 5)))
            {
                gAnimationIdx = 3;

                mCharacterAnimator.PlayAnimation(gAnimationIdx, true, 2.0f);
            }
            else if (ImGui::IsMouseHoveringRect(pointJump - ImVec2(5, 5), pointJump + ImVec2(5, 5)))
            {
                gAnimationIdx = 5;

                mCharacterAnimator.PlayAnimation(gAnimationIdx, true, 2.0f);
            }
            else if (ImGui::IsMouseHoveringRect(pointRightStrafe - ImVec2(5, 5), pointRightStrafe + ImVec2(5, 5)))
            {
                gAnimationIdx = 7;

                mCharacterAnimator.PlayAnimation(gAnimationIdx, true, 2.0f);
            }
            else if (ImGui::IsMouseHoveringRect(pointLeftStrafe - ImVec2(5, 5), pointLeftStrafe + ImVec2(5, 5)))
            {
                gAnimationIdx = 6;
                mCharacterAnimator.PlayAnimation(gAnimationIdx, true, 2.0f);
            }

            mouseX = mousePos.x;
            mouseY = mousePos.y;

        }
        
        ImGui::Text("Mouse X: %d", mouseX);
        ImGui::Text("Mouse Y: %d", mouseY);
        
        std::string state = "Idle";

        switch (gAnimationIdx)
        {
        case 2:
            state = "Idle";
            break;
        case 3:
            state = "Walk";
            break;
        case 5:
            state = "Jump";
            break;
        case 6:
            state = "Right Strage";
            break;
        case 7:
            state = "Left Strage";
            break;
        default:
            state = "Idle";
            break;
        }

        ImGui::Text("Character State: %s", state.c_str());

        ImGui::End();
    }
    ImGui::End();
}