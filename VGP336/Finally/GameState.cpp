#include "GameState.h"
#include "Input/Inc/InputSystem.h"\

using namespace GGEngine;
using namespace GGEngine::Graphic::AnimationUtil;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

void GameState::Initialize()
{
    mGameWorld.LoadLevel("../../Assets/Templates/Level/Test_Level_Final.json");
}

void GameState::Terminate()
{
    mGameWorld.Terminate();
}

void GameState::Update(float deltTime)
{
    mGameWorld.Update(deltTime);
}

Vector3 camerPos = Vector3::Zero;
void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    mGameWorld.DebugUI();
}

void GameState::EditorUI()
{
    mGameWorld.EditorUI();
}
