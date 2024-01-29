#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;
namespace {
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] = {
        Colors::Red,
        Colors::Blue,
        Colors::Yellow
        };

        index = (index + 1) % std::size(colorTable);

        return colorTable[index];
    }

    void CreateCubeIndicies(std::vector<uint32_t>& indicies)
    {
        indicies = {
            0,1,2,//|
            1,3,2,//|--- Front face

            1,7,3,//|
            1,5,7,//|--- Right face

            5,4,6,//|
            5,6,7,//|--- Back face

            4,0,2,//|
            4,2,6,//|--- Left face

            4,1,0,//|
            4,5,1,//|--- Top face

            3,7,6,//|
            2,3,6 //|--- Bottom face
        };
    }

    void CreatePlaneIndicies(std::vector<uint32_t>& indicies, int row, int col)
    {
        for (int r = 0; r < row; ++r)
        {
            for (int c = 0; c < col; ++c)
            {
                int i = r * (col + 1) + c;
                indicies.push_back(i);
                indicies.push_back(i + col + 1);
                indicies.push_back(i + 1);
                
                indicies.push_back(i + 1);
                indicies.push_back(i + col + 1);
                indicies.push_back(i + col + 2);
            }
        }
    }

    void CreateCapsIndicies(std::vector<uint32_t>& indicies, int slices, int topIdx)
    {
        int bottomIdx = topIdx + 1;
        for (uint32_t s = 0; s < slices; ++s)
        {        //Top Tri
            indicies.push_back(s);
            indicies.push_back(s + 1);
            indicies.push_back(bottomIdx);

            //Bottom Tri
            int topRow = topIdx - slices - 1 + s;
            indicies.push_back(topIdx);
            indicies.push_back(topRow + 1);
            indicies.push_back(topRow);
        }
    }

    void CreateCapsPC(MeshPC& mesh, int slices, float halfHeight)
    {
        int idx = rand() % 100;

        uint32_t topIdx = mesh.verticies.size();
        uint32_t bottomIdx = topIdx + 1;

        //TOP Center
        mesh.verticies.push_back({
            {0.0f,halfHeight,0.0f},
            GetNextColor(idx)
            });

        //Bottom Center
        mesh.verticies.push_back({
            {0.0f,-halfHeight,0.0f},
            GetNextColor(idx)
            });

        CreateCapsIndicies(mesh.indicies, slices, topIdx);
    }
}

MeshPC MeshBuilder::CreateCubePC(float size, Color color)
{
    MeshPC mesh;
    float hs = size / 2;
    //face
    mesh.verticies.push_back({ { -hs,  hs , -hs}, color }); //0 
    mesh.verticies.push_back({ {  hs,  hs , -hs}, color }); //1 
    mesh.verticies.push_back({ { -hs, -hs , -hs}, color }); //3 
    mesh.verticies.push_back({ {  hs, -hs , -hs}, color }); //2 
    //back
    mesh.verticies.push_back({ { -hs,  hs ,  hs}, color }); //4 
    mesh.verticies.push_back({ {  hs,  hs ,  hs}, color }); //5 
    mesh.verticies.push_back({ { -hs, -hs ,  hs}, color }); //6 
    mesh.verticies.push_back({ {  hs, -hs ,  hs}, color }); //7 

    mesh.indicies = {
        0,1,2,//|
        1,3,2,//|--- Front face

        1,7,3,//|
        1,5,7,//|--- Right face

        5,4,6,//|
        5,6,7,//|--- Back face

        4,0,2,//|
        4,2,6,//|--- Left face

        4,1,0,//|
        4,5,1,//|--- Top face

        3,7,6,//|
        2,3,6 //|--- Bottom face
    };

    return mesh;
}

MeshPC MeshBuilder::CreateRectanglePC(float length, float width, float depth)
{
    MeshPC mesh;

    int index = rand() % 100;
    float hw = width * 0.5;
    float hh = length * 0.5;
    float hd = depth * 0.5;
    int colr = 3;

    mesh.verticies.push_back({ { -hw,  hh , -hd}, GetNextColor(colr) }); //0 
    mesh.verticies.push_back({ {  hw,  hh , -hd}, GetNextColor(colr) }); //1 
    mesh.verticies.push_back({ { -hw, -hh , -hd}, GetNextColor(colr) }); //3 
    mesh.verticies.push_back({ {  hw, -hh , -hd}, GetNextColor(colr) }); //2 
    //back
    mesh.verticies.push_back({ { -hw,  hh ,  hd}, GetNextColor(colr) }); //4 
    mesh.verticies.push_back({ {  hw,  hh ,  hd}, GetNextColor(colr) }); //5 
    mesh.verticies.push_back({ { -hw, -hh ,  hd}, GetNextColor(colr) }); //6 
    mesh.verticies.push_back({ {  hw, -hh ,  hd}, GetNextColor(colr) }); //7 

    mesh.indicies = {
        0,1,2,//|
        1,3,2,//|--- Front face

        1,7,3,//|
        1,5,7,//|--- Right face

        5,4,6,//|
        5,6,7,//|--- Back face

        4,0,2,//|
        4,2,6,//|--- Left face

        4,1,0,//|
        4,5,1,//|--- Top face

        3,7,6,//|
        2,3,6 //|--- Bottom face
    };

    return mesh;
}

MeshPC MeshBuilder::CreatePlanePC(int row, int col, float space)
{
    MeshPC mesh;  

    float x = row * 0.5f * space;
    float y = col * 0.5f * space;

     mesh.verticies.push_back({ { +x, +y, 0.0f },Colors::Gray });
     mesh.verticies.push_back({ { +x, -y, 0.0f },Colors::Gray });
     mesh.verticies.push_back({ { -x, -y, 0.0f },Colors::Gray });
     mesh.verticies.push_back({ { -x, +y, 0.0f },Colors::Gray });

     mesh.indicies.emplace_back(0);
     mesh.indicies.emplace_back(1);
     mesh.indicies.emplace_back(3);
     mesh.indicies.emplace_back(3);
     mesh.indicies.emplace_back(1);
     mesh.indicies.emplace_back(2);
     mesh.indicies.emplace_back(2);
     mesh.indicies.emplace_back(1);
     mesh.indicies.emplace_back(0);
     mesh.indicies.emplace_back(2);
     mesh.indicies.emplace_back(0);
     mesh.indicies.emplace_back(3);

   
   return mesh;
}

MeshPC MeshBuilder::CreateCylinderPC(int slices, int rings) 
{
    MeshPC mesh;    
    int idx = rand() % 100;
    const float halfHeight = static_cast<float>(rings) * 0.5f;

    for (int r = 0; r <= rings; ++r)
    {
        float ring= static_cast<float>(r);

        for (int s = 0; s < slices + 1; ++s)
        {
            float slice = static_cast<float>(s);
            float rotation = (slice / static_cast<float>(slices)) * Constants::TwoPi;

            mesh.verticies.push_back({
                {
                     sin(rotation),
                     ring - halfHeight,
                     -cos(rotation)
                },
                GetNextColor(idx)
                });
        }
    }

    CreatePlaneIndicies(mesh.indicies, rings, slices);
    CreateCapsPC(mesh, slices, halfHeight);

    return mesh;
}

MeshPC MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
    int idx = rand() % 100;
    MeshPC mesh;
    
    float sliceAngleStep = Constants::TwoPi / static_cast<float>(slices);
    float ringAngleStep = Constants::Pi / static_cast<float>(rings - 1);
    
    for (uint32_t z = 0; z < rings; ++z)
    {
        float yy = sin((z * ringAngleStep) - (Constants::Pi / 2.0f)) * 0.5f;
        float radius = sqrt(0.25f - (yy * yy));
    
        for (uint32_t x = 0; x <= slices; ++x)
        {
            float xx = cos(x * sliceAngleStep) * radius;
            float zz = sin(x * sliceAngleStep) * radius;
    
            Vector3 position{ xx, yy, zz };
    
            mesh.verticies.emplace_back(position, GetNextColor(idx));
        }
    }
    
    for (uint32_t r = 0; r + 1 < rings; ++r)
    {
        for (uint32_t s = 0; s < slices; ++s)
        {
            mesh.indicies.push_back((s + 0) + ((r + 0) * (slices + 1)));
            mesh.indicies.push_back((s + 0) + ((r + 1) * (slices + 1)));
            mesh.indicies.push_back((s + 1) + ((r + 1) * (slices + 1)));
    
            mesh.indicies.push_back((s + 0) + ((r + 0) * (slices + 1)));
            mesh.indicies.push_back((s + 1) + ((r + 1) * (slices + 1)));
            mesh.indicies.push_back((s + 1) + ((r + 0) * (slices + 1)));
        }
    }
    
    
     return mesh;
   
//MeshPC mesh;
    //int idx = rand() % 100;
    //
    //for (int r = 0; r <= rings; ++r)
    //{
    //    float ring = static_cast<float>(r);
    //    float phi = ring * (Constants::Pi / static_cast<float>((rings - 1)));
    //
    //    for (int s = 0; s < slices + 1; ++s)
    //    {
    //        float slice = static_cast<float>(s);
    //        float rotation = (slice / static_cast<float>(slices)) * Constants::TwoPi;
    //
    //       
    //
    //        mesh.verticies.push_back({
    //            {
    //            radius * sin(rotation) * sin(phi),
    //            radius * cos(phi),
    //            radius * cos(rotation) * sin(phi)
    //            },
    //            GetNextColor(idx)
    //            });
    //    }
    //}
    //
    //CreatePlaneIndicies(mesh.indicies, rings, slices);
    //return mesh;
}

MeshPX MeshBuilder::CreateCubePX(float size)
{
    MeshPX mesh;
    float hs = size / 2;
    //face
    mesh.verticies.push_back({ { -hs,  hs , -hs}, { 0.25f, 0.33f } }); //0 
    mesh.verticies.push_back({ {  hs,  hs , -hs}, { 0.5f, 0.33f } }); //1 
    mesh.verticies.push_back({ {  hs, -hs , -hs}, { 0.5f, 0.66f } }); //2 
    mesh.verticies.push_back({ { -hs, -hs , -hs}, { 0.25f, 0.66f } }); //3 
    //back
    mesh.verticies.push_back({ { -hs,  hs ,  hs}, { 0.0f, 0.33f } }); //4 
    mesh.verticies.push_back({ {  hs,  hs ,  hs}, { 0.0f, 0.5f } }); //5 
    mesh.verticies.push_back({ {  hs, -hs ,  hs}, { 0.5f, 1.0f } }); //7 
    mesh.verticies.push_back({ { -hs, -hs ,  hs}, { 0.25f, 1.0f } }); //6 

    CreateCubeIndicies(mesh.indicies);
    
    return mesh;
}

MeshPX MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
    MeshPX mesh;
    int idx = rand() % 100;
    
    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);
    
    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        float phi = ring * (Constants::Pi / static_cast<float>((rings - 1)));

        for (int s = 0; s < slices + 1; ++s)
        {
            float slice = static_cast<float>(s);
            float rotation = (slice / static_cast<float>(slices)) * Constants::TwoPi;

            float u = 1.0f - (uStep * slice);
            float v = vStep * ring;

            mesh.verticies.push_back({
                {
                radius * sin(rotation) * sin(phi),
                radius * cos(phi),
                radius * cos(rotation) * sin(phi)
                },
                {u,v} 
                });
        }
    }

    CreatePlaneIndicies(mesh.indicies, rings, slices);
    return mesh;
}

Mesh MeshBuilder::CreateCube(float size)
{
    Mesh mesh;

    float hs = size * 0.5f;

    // FRONT
    mesh.verticies.push_back({ Vector3{  hs,  hs, -hs }, Vector3::ZAxis, Vector3::XAxis, Vector2(0.5f, 0.33f) }); // Top Right Close
    mesh.verticies.push_back({ Vector3{ -hs, -hs, -hs }, Vector3::ZAxis, Vector3::XAxis, Vector2(0.25f, 0.66f) }); // Bottom Left Close
    mesh.verticies.push_back({ Vector3{  hs, -hs, -hs }, Vector3::ZAxis, Vector3::XAxis, Vector2(0.5f, 0.66f) }); // Bottom Right Close

    mesh.verticies.push_back({ Vector3{  hs,  hs, -hs }, Vector3::ZAxis, Vector3::XAxis, Vector2(0.5f, 0.33f) }); // Top Right Close
    mesh.verticies.push_back({ Vector3{ -hs,  hs, -hs }, Vector3::ZAxis, Vector3::XAxis, Vector2(0.25f, 0.33f) }); // Top Left Close
    mesh.verticies.push_back({ Vector3{ -hs, -hs, -hs }, Vector3::ZAxis, Vector3::XAxis, Vector2(0.25f, 0.66f) }); // Bottom Left Close

    // BACK
    mesh.verticies.push_back({ Vector3{ -hs,  hs,  hs }, -Vector3::ZAxis, -Vector3::XAxis, Vector2(1.0f, 0.33f) }); // Top Left Far
    mesh.verticies.push_back({ Vector3{  hs, -hs,  hs }, -Vector3::ZAxis, -Vector3::XAxis, Vector2(0.75f, 0.66f) }); // Bottom Right Far
    mesh.verticies.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::ZAxis, -Vector3::XAxis, Vector2(1.0f, 0.66f) }); // Bottom Left Far

    mesh.verticies.push_back({ Vector3{ -hs,  hs,  hs }, -Vector3::ZAxis, -Vector3::XAxis, Vector2(1.0f, 0.33f) }); // Top Left Far
    mesh.verticies.push_back({ Vector3{  hs,  hs,  hs }, -Vector3::ZAxis, -Vector3::XAxis, Vector2(0.75f, 0.33f) }); // Top Right Far
    mesh.verticies.push_back({ Vector3{  hs, -hs,  hs }, -Vector3::ZAxis, -Vector3::XAxis, Vector2(0.75f, 0.66f) }); // Bottom Right Far

    // RIGHT
    mesh.verticies.push_back({ Vector3{  hs,  hs,  hs }, Vector3::XAxis, Vector3::ZAxis, Vector2(0.75f, 0.33f) }); // Top Right Far
    mesh.verticies.push_back({ Vector3{  hs, -hs, -hs }, Vector3::XAxis, Vector3::ZAxis, Vector2(0.5f, 0.66f) }); // Bottom Right Close
    mesh.verticies.push_back({ Vector3{  hs, -hs,  hs }, Vector3::XAxis, Vector3::ZAxis, Vector2(0.75f, 0.66f) }); // Bottom Right Far

    mesh.verticies.push_back({ Vector3{  hs,  hs,  hs }, Vector3::XAxis, Vector3::ZAxis, Vector2(0.75f, 0.33f) }); // Top Right Far
    mesh.verticies.push_back({ Vector3{  hs,  hs, -hs }, Vector3::XAxis, Vector3::ZAxis, Vector2(0.5f, 0.33f) }); // Top Right Close
    mesh.verticies.push_back({ Vector3{  hs, -hs, -hs }, Vector3::XAxis, Vector3::ZAxis, Vector2(0.5f, 0.66f) }); // Bottom Right Close

    // Left
    mesh.verticies.push_back({ Vector3{ -hs,  hs, -hs }, -Vector3::XAxis, -Vector3::ZAxis, Vector2(0.25f, 0.33f) }); // Top Left Close
    mesh.verticies.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::XAxis, -Vector3::ZAxis, Vector2(0.0f, 0.66f) }); // Bottom Left Far
    mesh.verticies.push_back({ Vector3{ -hs, -hs, -hs }, -Vector3::XAxis, -Vector3::ZAxis, Vector2(0.25f, 0.66f) }); // Bottom Left Close

    mesh.verticies.push_back({ Vector3{ -hs,  hs, -hs }, -Vector3::XAxis, -Vector3::ZAxis, Vector2(0.25f, 0.33f) }); // Top Left Close
    mesh.verticies.push_back({ Vector3{ -hs,  hs,  hs }, -Vector3::XAxis, -Vector3::ZAxis, Vector2(0.0f, 0.33f) }); // Top Left Far
    mesh.verticies.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::XAxis, -Vector3::ZAxis, Vector2(0.0f, 0.66f) }); // Bottom Left Far

    // Top
    mesh.verticies.push_back({ Vector3{  hs,  hs,  hs }, Vector3::YAxis, Vector3::XAxis, Vector2(0.5f,  0.0f) }); // Top Right Far
    mesh.verticies.push_back({ Vector3{ -hs,  hs, -hs }, Vector3::YAxis, Vector3::XAxis, Vector2(0.25f, 0.33f) }); // Top Left Close
    mesh.verticies.push_back({ Vector3{  hs,  hs, -hs }, Vector3::YAxis, Vector3::XAxis, Vector2(0.5f, 0.33f) }); // Top Right Close

    mesh.verticies.push_back({ Vector3{  hs,  hs,  hs }, Vector3::YAxis, Vector3::XAxis, Vector2(0.5f,  0.0f) }); // Top Right Far
    mesh.verticies.push_back({ Vector3{ -hs,  hs,  hs }, Vector3::YAxis, Vector3::XAxis, Vector2(0.25f,  0.0f) }); // Top Left Far
    mesh.verticies.push_back({ Vector3{ -hs,  hs, -hs }, Vector3::YAxis, Vector3::XAxis, Vector2(0.25f, 0.33f) }); // Top Left Close

    // Bottom
    mesh.verticies.push_back({ Vector3{  hs, -hs, -hs }, -Vector3::YAxis, -Vector3::XAxis, Vector2(0.5f, 0.66f) }); // Bottom Right Close
    mesh.verticies.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::YAxis, -Vector3::XAxis, Vector2(0.25f,  1.0f) }); // Bottom Left Far
    mesh.verticies.push_back({ Vector3{  hs, -hs,  hs }, -Vector3::YAxis, -Vector3::XAxis, Vector2(0.5f,  1.0f) }); // Bottom Right Far

    mesh.verticies.push_back({ Vector3{  hs, -hs, -hs }, -Vector3::YAxis, -Vector3::XAxis, Vector2(0.5f, 0.66f) }); // Bottom Right Close
    mesh.verticies.push_back({ Vector3{ -hs, -hs, -hs }, -Vector3::YAxis, -Vector3::XAxis, Vector2(0.25f, 0.66f) }); // Bottom Left Close
    mesh.verticies.push_back({ Vector3{ -hs, -hs,  hs }, -Vector3::YAxis, -Vector3::XAxis, Vector2(0.25f,  1.0f) }); // Bottom Left Far

    int i = 0;
    for (const auto& vertex : mesh.verticies)
    {
        mesh.indicies.push_back(i);
        ++i;
    }

    return mesh;
}

Mesh MeshBuilder::CreateSphere(int slices, int rings, float radius)
{
    Mesh mesh;

    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; ++r)
    {
        float ring = static_cast<float>(r);
        float phi = ring * (Constants::Pi / static_cast<float>((rings - 1)));

        for (int s = 0; s < slices + 1; ++s)
        {
            float slice = static_cast<float>(s);
            float rotation = (slice / static_cast<float>(slices)) * Constants::TwoPi;

            float u = 1.0f - (uStep * slice);
            float v = vStep * ring;

            float x = radius * sin(rotation) * sin(phi);
            float y = radius * cos(phi);
            float z = radius * cos(rotation) * sin(phi);
            const Vector3 pos = { x,y,z };
            const Vector3 norm = Normalize(pos);
            const Vector3 tan = Normalize({ -z, 0.0f,x });
            mesh.verticies.push_back({ pos,norm,tan,{u,v} });
        }
    }

    CreatePlaneIndicies(mesh.indicies, rings, slices);
    return mesh;
}

Mesh MeshBuilder::CreatePlane(int row, int col, float space)
{
    Mesh mesh;

    int idx = rand() % 100;
    const float hw = static_cast<float>(col) * space * 0.5f;
    const float hh= static_cast<float>(row) * space * 0.5f;

    float x = -hw;
    float z = -hh;

    const float uStep = 1.0f / static_cast<float>(col);
    const float vStep = 1.0f / static_cast<float>(row);

    const GGMath::Vector3 n = GGMath::Vector3::YAxis;
    const GGMath::Vector3 t = GGMath::Vector3::XAxis;

    for (int r = 0; r <= row; ++r)
    {
        float rf = static_cast<float>(r);
        for (int c = 0; c <= col; ++c)
        {
            float cf = static_cast<float>(c);
            float u = uStep * cf;
            float v = 1.0f - (vStep * rf);

            mesh.verticies.push_back({ {x,0.0f,z},n,t,GGMath::Vector2(u,v) });
            x += space;
        }

        x = -hw;
        z += space;
    }

    CreatePlaneIndicies(mesh.indicies, row, col);

    return mesh;
}


MeshPX GGEngine::Graphic::MeshBuilder::CreateScreenQuad()
{
    MeshPX mesh;
    mesh.verticies.push_back({ { -1.0f,-1.0f,0.0f }, {0.0f,1.0f} });
    mesh.verticies.push_back({ { -1.0f, 1.0f,0.0f }, {0.0f,0.0f} });
    mesh.verticies.push_back({ {  1.0f, 1.0f,0.0f }, {1.0f,0.0f} });
    mesh.verticies.push_back({ {  1.0f,-1.0f,0.0f }, {1.0f,1.0f} });
    mesh.indicies = { 0,1,2,0,2,3 };
    return mesh;
}
