#include <GGEngine/Inc/GGEngine.h>
#include "GameState.h"
#include <iostream>

enum class States :size_t
{
    GameState = 0
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GGEngine::App& mainApp = GGEngine::MainApp();

    GGEngine::AppConfig config;
    config.appName = L"Hello Reflection";
    config.winWidth = 1280;
    config.winHeight = 720;

    mainApp.AddState<GameState>((size_t)States::GameState);
    mainApp.Run(config);

    return 0;
}