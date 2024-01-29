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
    struct Vertex
    {
        GGEngine::GGMath::Vector3 position;
        GGEngine::Color color;
    };
    std::vector<Vertex> mVerticies;

    std::map<std::string, std::vector<Vertex>> mShape;
    std::vector<ID3D11Buffer*> mVertexBufferList;

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;

    int indx = 0;
};