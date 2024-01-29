#include "Precompiled.h"
#include "App.h"
#include "AppState.h"

//#define _IS_EDITOR

using namespace GGEngine;
using namespace GGEngine::Core;
using namespace GGEngine::Graphic;
using namespace GGEngine::Graphic::DebugUI;
using namespace GGEngine::Input;
using namespace GGEngine::Physics;

void App::ChangeState(size_t stateID)
{
    auto iter = mAppStates.find(stateID);
    if (iter != mAppStates.end())
    {
        mNextState = iter->second.get();
    }
}

void App::Run()
{
    //ASSERT(false, "NO LONGER USED");
    /// <summary>NO LONGER USED</summary>
    
    AppConfig config;
    Run(config);
}

void App::Run(const AppConfig& config)
{
    Window myWindow;
    myWindow.Initialize(
        GetModuleHandle(nullptr),
        config.appName,
        config.winWidth,
        config.winHeight);

    auto handle = myWindow.GetWindowHandle();
    InputSystem::StaticInitialize(handle);
    GraphicSystem::StaticInitialize(handle, false);
    DebugUI::StaticInitialize(handle, false, true);
    SimpleDraw::StaticInitalize(config.debugDrawLimit);
    TextureManger::StaticInitalize("../../Assets/");
    ModelManager::StaticInitialize();
    
    PhysicsWorld::Settings sets = {
    config.g,
    config.simulation,
    config.fixedTimeStep
    };
    PhysicsWorld::StaticInitialize(sets);

    ASSERT(mCurrentState, "App -- no app state found");
    mCurrentState->Initialize();

    
    mRunning = true;

    while (mRunning)
    {
        myWindow.ProcessMessage();

        auto inputSystem = Input::InputSystem::Get();
        inputSystem->Update();

        if (!myWindow.IsActive() || inputSystem->IsKeyPressed(Input::KeyCode::ESCAPE))
        {
            //mRunning = false;
            Quit();
            continue;
        }

        if (mNextState != nullptr)
        {
            mCurrentState->Terminate();
            mCurrentState = std::exchange(mNextState, nullptr);
            mCurrentState->Initialize();
        }
        auto deltaTime = TimeUtil::GetDeltaTime();
        if (deltaTime < 0.5f)
        {
#ifndef _IS_EDITOR
            PhysicsWorld::Get()->Update(deltaTime);
#endif // !_IS_EDITOR
            mCurrentState->Update(deltaTime);
        }
        
        auto graphicsSystem = GraphicSystem::Get();
        graphicsSystem->BeginRender();
            mCurrentState->Render();

#ifdef _IS_EDITOR
        DebugUI::BeginRender();
            mCurrentState->EditorUI();
        DebugUI::EndRender();
#else
            DebugUI::BeginRender();
                mCurrentState->DebugUI();
            DebugUI::EndRender();
#endif // !_IS_EDITOR           

        graphicsSystem->EndRender();
    }
    mCurrentState->Terminate();

    PhysicsWorld::StaticTerminate();
    TextureManger::StaticTerminate();
    ModelManager::StaticTerminate();
    SimpleDraw::StaticTerminate();
    DebugUI::StaticTerminate();
    GraphicSystem::StaticTerminate();
    InputSystem::StaticTerminate();
    myWindow.Terminate();
}

void App::Quit()
{
    mRunning = false;
}
