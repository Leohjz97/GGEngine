#include "Precompiled.h"
#include "Texture.h"

#include"GraphicSystem.h"
#include<DirectXTK/Inc/WICTextureLoader.h>

using namespace GGEngine;
using namespace GGEngine::Graphic;

void Texture::UnbindPS(uint32_t slot)
{
    static ID3D11ShaderResourceView* dummy = nullptr;
    GraphicSystem::Get()->GetContext()->PSSetShaderResources(slot, 1, &dummy);
}

Texture::~Texture()
{
    ASSERT(mShaderResView == nullptr, "Shader resource view was nor destory");
}

Texture::Texture(Texture&& other) noexcept
    :mShaderResView(other.mShaderResView)
{
    other.mShaderResView = nullptr;
}

Texture& Texture::operator=(Texture&& other) noexcept
{
    mShaderResView = other.mShaderResView;
    other.mShaderResView = nullptr;

    return *this;
}

void Texture::Initialize(const std::filesystem::path& fileName)
{
    auto device = GraphicSystem::Get()->GetDevice();
    auto context = GraphicSystem::Get()->GetContext();
    HRESULT hr = DirectX::CreateWICTextureFromFile(device, context, fileName.c_str(), nullptr, &mShaderResView);
    ASSERT(SUCCEEDED(hr), "Failed to load texture");
}

void Texture::Initialize(uint32_t width, uint32_t height, Format format)
{
    ASSERT(false, "not yet implemented");
}

void Texture::Terminate()
{
    SafeRelease(mShaderResView);
}

void Texture::BindVS(uint32_t slot) const
{
    auto context = GraphicSystem::Get()->GetContext();
    context->VSSetShaderResources(slot, 1, &mShaderResView);
}

void Texture::BindPS(uint32_t slot) const
{
    auto context = GraphicSystem::Get()->GetContext();
    context->PSSetShaderResources(slot, 1, &mShaderResView);
}

DXGI_FORMAT Texture::GetDXGIFormat(Format format)
{
    switch (format)
    {
    case Texture::Format::RGBA_U32:
        return DXGI_FORMAT_R32G32B32A32_UINT;
    case Texture::Format::RGBA_U8:
        return DXGI_FORMAT_R8G8B8A8_UNORM;
    }

    return DXGI_FORMAT_R8G8B8A8_UNORM;
}
