#include <GGEngine/Inc/GGEngine.h>
#include "GameState.h"

enum class States :size_t
{
    GameState = 0,
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    GGEngine::App& mainApp = GGEngine::MainApp();

    mainApp.AddState<GameState>((size_t)States::GameState);
    mainApp.Run();

    return 0;
}