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

    void CreateCube();
    void OnCreateShape();
    //void DebugUI() override;

private:
    struct Vertex
    {
        GGEngine::GGMath::Vector3 position;
        GGEngine::Color color;
    };

    std::vector<Vertex> mVerticies;

    Camera mCamera;
    ConstantBuffer mConstantBuffer;

    MeshBuffer mMeshBuffer;
    VertexShader mVertexShader;
    PiexlShader mPiexlShader;

    float Xspeed, Yspeed;
};