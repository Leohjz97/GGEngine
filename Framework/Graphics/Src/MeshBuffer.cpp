#include "Precompiled.h"
#include "MeshBuffer.h"
#include "GraphicSystem.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void MeshBuffer::Initialize(const void* verticies, uint32_t vertexSize, uint32_t vertexCount)
{
    CreateVertexBuffer(verticies, vertexSize, vertexCount);
}

void MeshBuffer::Initialize(const void* verticies, uint32_t vertexSize, uint32_t vertexCount, const uint32_t* indics, uint32_t indexCount)
{
    CreateVertexBuffer(verticies, vertexSize, vertexCount);
    CreateIndexBuffer(indics, indexCount);    
}

void MeshBuffer::Terminate()
{
    SafeRelease(mVertexBuffer);
    SafeRelease(mIndexBuffer);
}

void MeshBuffer::SetTopology(Topology topology)
{
    switch (topology)
    {
    case Topology::Points:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
        break;
    case Topology::Lines:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    case Topology::Triangles:
        mTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    }
}

void MeshBuffer::Update(const void* vertices, uint32_t vertexCount)
{
    mVertexCount = vertexCount;

    auto context = GraphicSystem::Get()->GetContext();

    D3D11_MAPPED_SUBRESOURCE resource;
    context->Map(mVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
    memcpy(resource.pData, vertices, vertexCount * mVertexSize);
    context->Unmap(mVertexBuffer, 0);
}

void MeshBuffer::Render()const
{
    auto context = GraphicSystem::Get()->GetContext();
    context->IASetPrimitiveTopology(mTopology);
    UINT offset = 0;
    context->IASetVertexBuffers(0, 1, &mVertexBuffer, &mVertexSize, &offset);
    if (mIndexBuffer != nullptr)
    {
        context->IASetIndexBuffer(mIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
        context->DrawIndexed(mIndexCount, 0, 0);
    }
    else
    {
        context->Draw((UINT)mVertexCount, 0);
    }
}

void MeshBuffer::CreateVertexBuffer(const void* verticies, uint32_t vertexSize, uint32_t vertexCount)
{
    mVertexSize = vertexSize;
    mVertexCount = vertexCount;

    auto device = GraphicSystem::Get()->GetDevice();
    //=======================================================================
    //Create the vertex buffer
    const bool isDynamic = (verticies == nullptr);

    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = static_cast<UINT>(vertexCount * vertexSize);
    bufferDesc.Usage = isDynamic ? D3D11_USAGE_DYNAMIC : D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = isDynamic ? D3D11_CPU_ACCESS_WRITE : 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = verticies;

    HRESULT hr = device->CreateBuffer(&bufferDesc, (isDynamic ? nullptr : &initData), &mVertexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
}

void MeshBuffer::CreateIndexBuffer(const uint32_t* indics, uint32_t indexCount)
{
    mIndexCount = indexCount;

    auto device = GraphicSystem::Get()->GetDevice();
    //=======================================================================
    //Create the vertex buffer
    D3D11_BUFFER_DESC bufferDesc{};
    bufferDesc.ByteWidth = static_cast<UINT>(indexCount * sizeof(uint32_t));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = indics;

    HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, &mIndexBuffer);
    ASSERT(SUCCEEDED(hr), "Failed to create vertex buffer");
}
