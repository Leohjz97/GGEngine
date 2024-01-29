#include "Precompiled.h"
#include "Sampler.h"

#include"GraphicSystem.h"

using namespace GGEngine::Graphic;

namespace
{
    D3D11_FILTER GetFilter(Samlper::Filter filter)
    {
        switch (filter)
        {
        case Samlper::Filter::Point:
            return D3D11_FILTER_MIN_MAG_MIP_POINT;
        case Samlper::Filter::Linear:
            return D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        case Samlper::Filter::Anisotropic:
            return D3D11_FILTER_ANISOTROPIC;
        }

        return D3D11_FILTER_MIN_MAG_MIP_POINT;
    }

    D3D11_TEXTURE_ADDRESS_MODE GetAddressMode(Samlper::AddressMode mode)
    {
        switch (mode)
        {
        case GGEngine::Graphic::Samlper::AddressMode::Border:
            return D3D11_TEXTURE_ADDRESS_BORDER;
        case GGEngine::Graphic::Samlper::AddressMode::Clamp:
            return D3D11_TEXTURE_ADDRESS_CLAMP;
        case GGEngine::Graphic::Samlper::AddressMode::Mirror:
            return D3D11_TEXTURE_ADDRESS_MIRROR;
        case GGEngine::Graphic::Samlper::AddressMode::Wrap:
            return D3D11_TEXTURE_ADDRESS_WRAP;
        }

        return D3D11_TEXTURE_ADDRESS_BORDER;
    }
}

    Samlper::~Samlper()
    {
        ASSERT(mSamlper == nullptr, "Samlper was not released");
    }

    void Samlper::Initialize(Filter filter, AddressMode addressMode)
    {
        auto d3dFilter = GetFilter(filter);
        auto d3dAddressMode = GetAddressMode(addressMode);

        D3D11_SAMPLER_DESC desc{};
        desc.Filter = d3dFilter;
        desc.AddressU = d3dAddressMode;
        desc.AddressV = d3dAddressMode;
        desc.AddressW = d3dAddressMode;
        desc.BorderColor[0] = 1.0f;
        desc.BorderColor[1] = 1.0f;
        desc.BorderColor[2] = 1.0f;
        desc.BorderColor[3] = 1.0f;
        desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
        desc.MinLOD = 0;
        desc.MaxLOD = D3D11_FLOAT32_MAX;

        auto device = GraphicSystem::Get()->GetDevice();
        auto hr = device->CreateSamplerState(&desc, &mSamlper);
        ASSERT(SUCCEEDED(hr), "Failed to create sampler state");
    }

    void Samlper::Terminate()
    {
        SafeRelease(mSamlper);
    }

    void Samlper::BindVS(uint32_t slot)const
    {
        auto context = GraphicSystem::Get()->GetContext();
        context->VSSetSamplers(slot, 1, &mSamlper);
    }

    void Samlper::BindPS(uint32_t slot)const
    {
        auto context = GraphicSystem::Get()->GetContext();
        context->PSSetSamplers(slot, 1, &mSamlper);
    }
