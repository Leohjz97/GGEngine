#include "Precompiled.h"
#include "CharacterContollerComponent.h"

#include "GameObject.h"
#include "GameWorld.h"
#include "AnimatorComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"

using namespace GGEngine;
using namespace GGEngine::Input;
using namespace GGEngine::GGMath;

void CharacterControllerComponent::Initialize()
{
    mTransformComp = GetOwner().GetComponent<TransformComponent>();
    mAnimatorComp = GetOwner().GetComponent<AnimatorComponent>();
}

void CharacterControllerComponent::Update(float deltaTime)
{
    auto input = InputSystem::Get();
    mSprintSpeed = mWalkSpeed * mSprintSpeedMult;
    moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? mSprintSpeed : mWalkSpeed;
    mTurnSpeed = 0.1f;

     transform = mTransformComp->GetMatrix4();
     vel = Vector3::Zero;

    //Baisc Move
    if (input->IsKeyDown(KeyCode::W))
    {
        vel = GetLookAt((transform) * moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        vel = GetRight((transform) * -moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        vel = GetLookAt((transform) * -moveSpeed);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        vel = GetRight((transform)*moveSpeed);
    }

    newPosition = GetTranslation(transform);
    newPosition += vel * deltaTime;
    mTransformComp->position = newPosition;
    //rb->SetPosition(newPosition);

    static float rotation = 0.0f;
    //Rotationw
    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        auto mouseX = input->GetMouseMoveX() * deltaTime * Constants::HalfPi * 0.125f;
        rotation += mouseX;
        mTransformComp->rotation = Quaternion::CreateFromYawPitchRoll(rotation, 0.0f, 0.0f);
    }

    //Switch Animation
    if (MagnitudeSqr(vel) > 0.0f)
    {
        if (moveSpeed > mWalkSpeed)
        {
            if (currentState != CharacterAnimations::Sprint)
            {
                currentState = CharacterAnimations::Sprint;
                mAnimatorComp->PlayAnimation(currentState, true);
            }
        } 
        //else if (vel.x > 0 && currentState != CharacterAnimations::Left)
        //{
        //    currentState = CharacterAnimations::Left;
        //    mAnimatorComp->PlayAnimation(currentState, true);
        //}
        //else if (vel.x < 0 && currentState != CharacterAnimations::Right)
        //{
        //    currentState = CharacterAnimations::Right;
        //    mAnimatorComp->PlayAnimation(currentState, true);
        //}
        //else if (vel.z < 0 && currentState != CharacterAnimations::Back)
        //{
        //    currentState = CharacterAnimations::Back;
        //    mAnimatorComp->PlayAnimation(currentState, true);
        //}
        else if(currentState != CharacterAnimations::Walk)
        {  
            currentState = CharacterAnimations::Walk;
            mAnimatorComp->PlayAnimation(currentState, true);;
        }
    }
    else if (currentState != CharacterAnimations::Idle)
    {
        currentState = CharacterAnimations::Idle;
        mAnimatorComp->PlayAnimation(currentState, true);
    }
}

void GGEngine::CharacterControllerComponent::DebugUI()
{
    if (ImGui::CollapsingHeader("Player", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::DragFloat("Turn Speed", &mTurnSpeed, 1.0, 1.0, 20.0f);
        ImGui::DragFloat("Walk Speed", &mWalkSpeed, 1.0, 1.0, 800.0f);
        ImGui::DragFloat("Sprint Speed Mult", &mSprintSpeedMult, 1.0, 1.0, 10.0f);

        ImGui::Text("Velocity: %.2f, %.2f, %.2f", vel.x, vel.y, vel.z);
        ImGui::Text("Position: %.2f, %.2f, %.2f", newPosition.x, newPosition.y, newPosition.z);
        ImGui::Text("Rotation: %.2f, %.2f, %.2f", mTransformComp->rotation.x, mTransformComp->rotation.y, mTransformComp->rotation.z);
        ImGui::Text("Current State:");
        const char* animationNames[] = { "Skeleton", "Dancing", "Idle", "Walk", "Sprint", "Left", "Right", "Back"};
        ImGui::Combo("##CharacterStateCombo", (int*)&currentState, animationNames, CharacterAnimations::Count);

    }
}

void GGEngine::CharacterControllerComponent::ParseCharacterControlComponent(const rj::Value& data, GameObject& obj)
{
    auto characterControllerComponent = obj.AddComponent<CharacterControllerComponent>();

    if (data.HasMember("WalkSpeed"))
    {
        const float mWalkSpeed = data["WalkSpeed"].GetFloat();
        characterControllerComponent->SetWalkSpeed(mWalkSpeed);
    }

    if (data.HasMember("SprintSpeed"))
    {
        const float mSprintSpeed = data["SprintSpeed"].GetFloat();
        characterControllerComponent->SetSprintSpeed(mSprintSpeed);
    }
}
