#include "GameState.h"
#include "Input/Inc/InputSystem.h"
#include "AnimationChangeComoponent.h"
#include "AnimationChangeService.h"

using namespace GGEngine;
using namespace GGEngine::Graphic::AnimationUtil;
using namespace GGEngine::GGMath;
using namespace GGEngine::Input;

namespace
{
    bool CustomGameMake(const char* componentName, const rapidjson::Value& value, GameObject& obj)
    {
        if (strcmp(componentName,"AnimationChangeComponent") == 0)
        {
            auto* animationChangeComponent = obj.AddComponent<AnimationChangeComponent>();
            return true;
        }
        return false;
    }

    bool CustomServiceMake(const char* serviceName, const rapidjson::Value& value, GameWorld& world)
    {
        if (strcmp(serviceName, "AnimationChangeService") == 0)
        {
            auto* animationChangeService = world.AddService<AnimationChangeService>();
            return true;
        }
        return false;
    }
}

void GameState::Initialize()
{
    GameObjectFactory::SetCustomMake(CustomGameMake);
    GameWorld::SetCustomServiceMake(CustomServiceMake);

    mGameWorld.LoadLevel("../../Assets/Templates/Level/Test_Level.json");
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
