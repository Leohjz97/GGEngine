#include "Precompiled.h"
#include"GraphicSystem.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

namespace
{
    std::unique_ptr<GraphicSystem> mGraphicSystem;
    Core::WindowMessageHandler mWindowMessageHandler;

}

void GraphicSystem::StaticInitialize(HWND window, bool fullscreen)
{
    ASSERT((mGraphicSystem == nullptr), "[ERROR] GraphicSystem is already initialized!");
    mGraphicSystem = std::make_unique<GraphicSystem>();
    mGraphicSystem->Initalize(window, fullscreen);
}

void GraphicSystem::StaticTerminate()
{
    if (mGraphicSystem != nullptr)
    {
        mGraphicSystem->Terminate();
        mGraphicSystem.reset();
    }
}

GraphicSystem* GraphicSystem::Get()
{
    ASSERT((mGraphicSystem != nullptr), "[ERROR] GraphicSystem is not created!");
    return  mGraphicSystem.get();
}

GraphicSystem::~GraphicSystem()
{
    ASSERT((mD3DDevice == nullptr), "[ERROR] GraphicSystem Terminate() must be called to clean up!");
}

void GraphicSystem::Initalize(HWND window, bool fullscreen)
{
    RECT clientRect = {};
    GetClientRect(window, &clientRect);
    UINT width = (UINT)(clientRect.right - clientRect.left);
    UINT height = (UINT)(clientRect.bottom - clientRect.top);

    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.OutputWindow = window;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = !fullscreen;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    HRESULT hr = D3D11CreateDeviceAndSwapChain
    (
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        0,
        &featureLevel,
        1,
        D3D11_SDK_VERSION,
        &swapChainDesc,
        &mSwapChain,
        &mD3DDevice,
        nullptr,
        &mImmediateContext
    );
    ASSERT((SUCCEEDED(hr)), "[ERROR] failed to initialuze device or swap chain");

    mSwapChain->GetDesc(&mSwapChainDesc);
    Resize(GetBackBufferWidth(), GetBackBufferHeight());
    //mWindowMessageHandler.Hook(window, GraphicSystemMEssageHandler);
}

void GraphicSystem::Terminate()
{
    LOG("GraphicsSystem -- Terminating...");

    // Restore original window's procedure
    mWindowMessageHandler.UnHook();

    SafeRelease(mDepthStencilView);
    SafeRelease(mDepthStencilBuffer);
    SafeRelease(mRenderTargetView);
    SafeRelease(mSwapChain);
    SafeRelease(mImmediateContext);
    SafeRelease(mD3DDevice);

    LOG("GraphicsSystem -- System terminated.");
}

void GraphicSystem::BeginRender()
{
    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
    mImmediateContext->ClearRenderTargetView(mRenderTargetView, (FLOAT*)(&mClearColor));
    mImmediateContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void GraphicSystem::EndRender()
{
    mSwapChain->Present(mSync, 0);
}

void GraphicSystem::ToggleFullscreen()
{
    BOOL fullScreen;
    mSwapChain->GetFullscreenState(&fullScreen, nullptr);
    mSwapChain->SetFullscreenState(!fullScreen, nullptr);
}

void GraphicSystem::Resize(uint32_t width, uint32_t height)
{
    mImmediateContext->OMSetRenderTargets(0, nullptr, nullptr);

    SafeRelease(mRenderTargetView);
    SafeRelease(mDepthStencilView);
    HRESULT hr;
    if (width != GetBackBufferWidth() || height != GetBackBufferHeight())
    {
        hr = mSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
        ASSERT(SUCCEEDED(hr), "[ERROR] failed to resize swap chain buffer");

        mSwapChain->GetDesc(&mSwapChainDesc);
    }

    ID3D11Texture2D* backBuffer = nullptr;
    hr = mSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    ASSERT(SUCCEEDED(hr), "[ERROR] failed to create render Chain view");

    hr = mD3DDevice->CreateRenderTargetView(backBuffer, nullptr, &mRenderTargetView);
    SafeRelease(backBuffer);
    ASSERT(SUCCEEDED(hr), "[ERROR] failed create render target view");

    //Depth stencil buffer
    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = GetBackBufferWidth();
    descDepth.Height = GetBackBufferHeight();
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    descDepth.CPUAccessFlags = 0;
    descDepth.MiscFlags = 0;
    hr = mD3DDevice->CreateTexture2D(&descDepth, nullptr, &mDepthStencilBuffer);
    ASSERT(SUCCEEDED(hr), "[ERROR] failed create depth stencil buffer");

    //Depth stencil view
    D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    descDSV.Texture2D.MipSlice = 0;
    hr = mD3DDevice->CreateDepthStencilView(mDepthStencilBuffer, &descDSV, &mDepthStencilView);
    ASSERT(SUCCEEDED(hr), "[ERROR] failed create depth stencil view");

    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

    //setup the viewport
    mViewport.Width = static_cast<float>(GetBackBufferWidth());
    mViewport.Height = static_cast<float>(GetBackBufferHeight());
    mViewport.MinDepth = 0.0f;
    mViewport.MaxDepth = 1.0f;
    mViewport.TopLeftX = 0;
    mViewport.TopLeftY = 0;
    mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicSystem::ResetRenderTarget()
{
    mImmediateContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);
}

void GraphicSystem::ResetViewport()
{
    mImmediateContext->RSSetViewports(1, &mViewport);
}

void GraphicSystem::SetClearColor(Color clearColor)
{
    mClearColor = clearColor;
}

void GraphicSystem::SetSync(bool sync)
{
    sync = mSync ? 1 : 0;
}

LRESULT GraphicSystem::GraphicSystemMEssageHandler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (mGraphicSystem)
    {
        switch (message)
        {
            case WM_SIZE: 
            {
                const uint32_t width = static_cast<uint32_t>(LOWORD(lParam));
                const uint32_t height = static_cast<uint32_t>(LOWORD(lParam));
                mGraphicSystem->Resize(width, height);
                break;
            }
        }           
    }
    return mWindowMessageHandler.ForwardMessage(window, message, wParam, lParam);
}

uint32_t GraphicSystem::GetBackBufferWidth() const
{
    return mSwapChainDesc.BufferDesc.Width;
}

uint32_t GraphicSystem::GetBackBufferHeight() const
{
    return mSwapChainDesc.BufferDesc.Height;
}

float GraphicSystem::GetBackBufferAspectRatio() const
{
    return static_cast<float>(GetBackBufferWidth()) / static_cast<float>(GetBackBufferHeight());
}
