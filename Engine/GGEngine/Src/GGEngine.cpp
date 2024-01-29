#include "Precompiled.h"
#include "GGEngine.h"

GGEngine::App& GGEngine::MainApp()
{
    static App mApp;
    return mApp;
}