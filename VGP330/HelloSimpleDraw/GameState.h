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

    bool mIsLine = true;
    Vector3 mLinesP1 = Vector3::Zero;
    Vector3 mLinesP2 = Vector3::One;
    Color mLineColor = Colors::Azure;

    int mSlice = 10;
    int mRings = 10;
    float mCircleRad = 1.0f;
    Color mCircleColor = Colors::Azure;
};