#pragma once
#include <GGEngine/Inc/GGEngine.h>

using namespace GGEngine;
using namespace GGEngine::Graphic;

class GameState : public GGEngine::AppState
{
public:
    virtual ~GameState() = default;

    void Initialize() override;
    void Terminate() override;

    void Update(float deltTime) override;
    void Render() override;
    void DebugUI() override;

private:
    void ShowNormal();

    Camera mCamera;
    DirectionaLight light;
    
    Mesh sphere;
    RenderGroup mModel;
    RenderObject mPlane;

    //RenderTarget mRenderTarget;

    StandardEffect mStandardEffect;
    //PostProcessingEffect mPostProcessingEffect;

    bool mShowNormal = false;
};