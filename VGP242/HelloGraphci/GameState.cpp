#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;

void GameState::Initialize()
{
	Graphic::GraphicSystem::Get()->SetClearColor(Colors::Black);
}

void GameState::Update(float deltTime)
{
	auto inputSystem = Input::InputSystem::Get();
	if (inputSystem->IsKeyPressed(Input::KeyCode::NUMPAD1))
	{
		Graphic::GraphicSystem::Get()->SetClearColor(Colors::WhiteSmoke);
	}
	else if (inputSystem->IsKeyPressed(Input::KeyCode::NUMPAD2))
	{
		Graphic::GraphicSystem::Get()->SetClearColor(Colors::BlueViolet);
	}
}