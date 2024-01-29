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
    //void DebugUI() override;

private:
    Camera mCamera;
    ConstantBuffer mConstantBuffer;

    //MeshPC mesh;
    MeshPX sphere;
    textureID mEarthID;
    MeshBuffer mMeshBuffer;
    VertexShader mVertexShader;
    PiexlShader mPiexlShader;

    float Xspeed, Yspeed;
};