#pragma once
#include "MeshType.h"

namespace GGEngine::Graphic 
{
    class MeshBuilder
    {
    public:
        //Postion and Color
        static MeshPC CreateCubePC(float size, Color color);
        static MeshPC CreateRectanglePC(float length, float width, float depth);
        static MeshPC CreatePlanePC(int numRows, int numCols, float space);
        static MeshPC CreateCylinderPC(int slics, int rings);
        static MeshPC CreateSpherePC(int slics, int rings, float radius);

        //Postion and UV
        static MeshPX CreateCubePX(float size);
        static MeshPX CreateSpherePX(int slices, int rings, float radius);

        //Position Normal Tangent UV
        static Mesh CreateCube(float size);
        static Mesh CreateSphere(int slices, int rings, float radius);
        static Mesh CreatePlane(int row, int col, float space);

        static MeshPX CreateScreenQuad();
    };
}