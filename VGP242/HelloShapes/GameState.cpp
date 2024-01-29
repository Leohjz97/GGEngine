#include "GameState.h"
#include "Input/Inc/InputSystem.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

void GameState::Initialize()
{
    // NDC - Normalized Device Coordinate
    //
    //            +-----------------+
    //           /                 /|
    //     1.0  +-----------------+ |
    //          |        ^        | |
    //          |        |        | |
    //          | <------+------> | |
    //          |        |        | | 1.0
    //          |        v        |/
    //    -1.0  +-----------------+ 0.0
    //      -1.0               1.0
    //
    //shape1
    mVerticies.push_back({ GGMath::Vector3(0.0f, 0.5f, 0.0f), Colors::Blue });
    mVerticies.push_back({ GGMath::Vector3(0.5f, -0.5f, 0.0f), Colors::Red });
    mVerticies.push_back({ GGMath::Vector3(-0.5f, -0.5f, 0.0f), Colors::Yellow });
    mShape["Shape1"] = mVerticies;
    mVerticies.clear();

    //shape2
    mVerticies.push_back({ Vector3{ 0.00f,  0.70f, 0.0f },Colors::Red });
    mVerticies.push_back({ Vector3{ 0.29f,  0.00f, 0.0f } ,Colors::Yellow });
    mVerticies.push_back({ Vector3{ -0.29f,  0.00f, 0.0f },Colors::Blue });

    mVerticies.push_back({ Vector3{  0.29f,  0.00f, 0.0f },Colors::Red });
    mVerticies.push_back({ Vector3{  0.58f, -0.70f, 0.0f } ,Colors::Yellow });
    mVerticies.push_back({ Vector3{  0.00f, -0.70f, 0.0f },Colors::Blue });

    mVerticies.push_back({ Vector3{  -0.29f,  0.00f, 0.0f } ,Colors::Red });
    mVerticies.push_back({ Vector3{  0.00f, -0.70f, 0.0f },Colors::Yellow });
    mVerticies.push_back({ Vector3{ -0.58f, -0.70f, 0.0f },Colors::Blue });
    mShape["Shape2"] = mVerticies;
    mVerticies.clear();

    //shape3
    mVerticies.push_back({ Vector3{  0.00f,  0.80f, 0.0f  },Colors::Green });
    mVerticies.push_back({ Vector3{  0.20f,  0.55f, 0.0f } ,Colors::Green });
    mVerticies.push_back({ Vector3{  -0.20f,  0.55f, 0.0f },Colors::Green });

    mVerticies.push_back({ Vector3{ 0.00f,  0.65f, 0.0f },Colors::Green });
    mVerticies.push_back({ Vector3{ 0.27f,  0.30f, 0.0f } ,Colors::Green });
    mVerticies.push_back({ Vector3{ -0.27f,  0.30f, 0.0f },Colors::Green });

    mVerticies.push_back({ Vector3{  0.00f,  0.40f, 0.0f },Colors::Green });
    mVerticies.push_back({ Vector3{  0.35f, -0.00f, 0.0f } ,Colors::Green });
    mVerticies.push_back({ Vector3{  -0.35f, -0.00f, 0.0f },Colors::Green });

    mVerticies.push_back({ Vector3{  -0.05f,  0.00f, 0.0f },Colors::SandyBrown });
    mVerticies.push_back({ Vector3{  0.05f,  0.00f, 0.0f },Colors::Brown });
    mVerticies.push_back({ Vector3{  -0.05f, -0.45f, 0.0f },Colors::Brown });

    mVerticies.push_back({ Vector3{  0.05f,  0.00f, 0.0f },Colors::Brown });
    mVerticies.push_back({ Vector3{  0.05f, -0.45f, 0.0f },Colors::Brown });
    mVerticies.push_back({ Vector3{  -0.05f, -0.45f, 0.0f },Colors::Brown });
    mShape["Shape3"] = mVerticies;
    mVerticies.clear();

        auto device = GraphicSystem::Get()->GetDevice();
        //=======================================================================
        //Create the vertex buffer
        HRESULT hr;
        int i = 0;
        for (auto it = mShape.begin(); it != mShape.end(); ++it)
        {
            D3D11_BUFFER_DESC bufferDesc{};
            bufferDesc.ByteWidth = it->second.size() * sizeof(Vertex);
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
            bufferDesc.MiscFlags = 0;
            bufferDesc.StructureByteStride = 0;

            D3D11_SUBRESOURCE_DATA initData{};
            initData.pSysMem = it->second.data();

            mVertexBufferList.emplace_back();
            hr = device->CreateBuffer(&bufferDesc, &initData, &mVertexBufferList.back());
            ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
        }
        mVertexBuffer = mVertexBufferList.front();
        indx = mShape["shape1"].size();

        //=======================================================================
        //Create the vertex Shader
        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
        ID3DBlob* shaderBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;
        hr = D3DCompileFromFile(
            L"../../Assets/Shaders/DoSomething.fx",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "VS", "vs_5_0",
            shaderFlags, 0, &shaderBlob, &errorBlob
        );

        if (errorBlob && errorBlob->GetBufferPointer())
        {
            LOG("%s", static_cast<const char*> (errorBlob->GetBufferPointer()));
        }
        ASSERT(SUCCEEDED(hr), "Failed to compile vertex shader");

        hr = device->CreateVertexShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &mVertexShader
        );
        ASSERT(SUCCEEDED(hr), "Failed to create vertex shader");

        //======================================================================
        //Create the Input
        std::vector<D3D11_INPUT_ELEMENT_DESC> vertexLayout;
        vertexLayout.push_back({ "POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });
        vertexLayout.push_back({ "COLOR",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT });

        hr = device->CreateInputLayout(
            vertexLayout.data(),
            (UINT)vertexLayout.size(),
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            &mInputLayout
        );
        ASSERT(SUCCEEDED(hr), "Failed to create input layout");

        SafeRelease(errorBlob);
        SafeRelease(shaderBlob);

        //=====================================================================
        //Create the Pixel Shader
        hr = D3DCompileFromFile(
            L"../../Assets/Shaders/DoSomething.fx",
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            "PS", "ps_5_0",
            shaderFlags, 0, &shaderBlob, &errorBlob
        );
        if (errorBlob && errorBlob->GetBufferPointer())
        {
            LOG("%s", static_cast<const char*> (errorBlob->GetBufferPointer()));
        }
        ASSERT(SUCCEEDED(hr), "Failed to compile pixel shader");

        hr = device->CreatePixelShader(
            shaderBlob->GetBufferPointer(),
            shaderBlob->GetBufferSize(),
            nullptr,
            &mPixelShader
        );
        ASSERT(SUCCEEDED(hr), "Failed to create pixel shader");
        SafeRelease(shaderBlob);
        SafeRelease(errorBlob);
}

void GameState::Terminate()
{
	SafeRelease(mVertexBuffer);
	SafeRelease(mVertexShader);
	SafeRelease(mInputLayout);
	SafeRelease(mPixelShader);
    for (int i = 0; i < mVertexBufferList.size(); i++)
        SafeRelease(mVertexBufferList[i]);
}

void GameState::Update(float deltTime)
{
    auto inputSystem = Input::InputSystem::Get();
    if (inputSystem->IsKeyPressed(Input::KeyCode::NUMPAD1) ||
        inputSystem->IsKeyPressed(Input::KeyCode::ONE))
    {
        mVertexBuffer = mVertexBufferList[0];
        indx = mShape["Shape1"].size();
    }
    if (inputSystem->IsKeyPressed(Input::KeyCode::NUMPAD2) || 
        inputSystem->IsKeyPressed(Input::KeyCode::TWO))
    {
        mVertexBuffer = mVertexBufferList[1];
        indx = mShape["Shape2"].size();
    }
    if (inputSystem->IsKeyPressed(Input::KeyCode::NUMPAD3) ||
        inputSystem->IsKeyPressed(Input::KeyCode::THREE))
    {
        mVertexBuffer = mVertexBufferList[2];
        indx = mShape["Shape3"].size();
    }
}

void GameState::Render()
{
    auto context = GraphicSystem::Get()->GetContext();
    
    context->VSSetShader(mVertexShader, nullptr, 0);
    context->IASetInputLayout(mInputLayout);
    context->PSSetShader(mPixelShader, nullptr, 0);
    
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &stride, &offset);
    context->Draw(indx, 0);
}

//void DebugUI()
//{
//
//}