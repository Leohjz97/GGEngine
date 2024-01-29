#pragma once
#include"VertexTypes.h"

namespace GGEngine::Graphic
{
    template<class VertexT>
    struct MeshBase
    {
        using VertexType = VertexT;
        std::vector<VertexType> verticies;
        std::vector<uint32_t> indicies;
    };

    using MeshP = MeshBase<VertexP>;
    using MeshPC = MeshBase<VertexPC>;
    using MeshPX = MeshBase<VertexPX>;
    using Mesh = MeshBase<Vertex>;
}