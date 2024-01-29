#include "Precompiled.h"
#include "PixelShader.h"
#include "GraphicSystem.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void PiexlShader::Initialize(const std::filesystem::path& filePath)
{
    //=====================================================================
//Create the Pixel Shader
    auto device = GraphicSystem::Get()->GetDevice();
    DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
    ID3DBlob* shaderBlob = nullptr;
    ID3DBlob* errorBlob = nullptr;
    HRESULT hr = D3DCompileFromFile(
        filePath.c_str(),
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

void PiexlShader::Terminate()
{
    SafeRelease(mPixelShader);
}

void PiexlShader::Bind()
{
    auto context = GraphicSystem::Get()->GetContext();
    context->PSSetShader(mPixelShader, nullptr, 0);
}