#pragma once
#include <GGEngine/Inc/GGEngine.h>

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::Physics;

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
    
    RenderObject mBall;
    RigidBody rb;
    CollisionShapeSphere spherC;

    RenderObject mGround;
    RigidBody rbGround;
    CollisionShapeBox groundCP;

    StandardEffect mStandardEffect;
};