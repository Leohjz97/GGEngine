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
    Camera mCamera;
    DirectionaLight mDirectionalLight;
    
    //Mesh sphere;
    RenderObject mSphere;
    RenderObject mPlane;
    StandardEffect mStandardEffect;

    //Animation
    Animation mSphereAnimation;
    float animationTime;
};