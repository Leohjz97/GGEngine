#pragma once
#include <GGEngine/Inc/GGEngine.h>

using namespace GGEngine;
using namespace GGEngine::Graphic;
using namespace GGEngine::GGMath;

class GameState : public GGEngine::AppState
{
public:
    virtual ~GameState() = default;

    void Initialize() override;
    void Terminate() override;

    void Update(float deltTime) override;
    void Render() override;
    void DebugUI() override;

private:
    //void ShowNormal();
    struct TransformData
    {
        Matrix4 wvp;
    };
    
    struct ReflectionTransformData
    {
        Matrix4 world;
        Matrix4 view;
        Matrix4 porj;
    };

    struct ReflectionBufferData
    {
        Matrix4 reflectionM;
    };

    struct PowerBufferData 
    {
        float power = 0.0f;
        float padding[3] = {};
    };

    Camera mCamera;
    Camera mReflectionCamera;

    DirectionaLight mDirectionalLight;
   
    RenderTarget mReflectRenderTarget;

    using TransformBuffer = TypedConstantBuffer<TransformData>;
    using ReflectionTransformDataBuffer = TypedConstantBuffer<ReflectionTransformData>;
    using ReflectionBuffer = TypedConstantBuffer<ReflectionBufferData>;
    using PowerBuffer = TypedConstantBuffer<PowerBufferData>;

    TransformBuffer mTransformBuffer;
    ReflectionTransformDataBuffer mReflectionTransformBuffer;
    ReflectionBuffer mReflectionBuffer;
    PowerBuffer mPowerBuffer;

    Mesh mGround;
    MeshBuffer mGroundMB;
    VertexShader mGroundVS;
    PiexlShader mGroundPS;
    Texture mGroundTexture;

    Mesh mSphere;
    MeshBuffer mSphereMB;
    VertexShader mSphereVS;
    PiexlShader mSpherePS;
    Texture mSphereTexture;

    Samlper mSampler;

    Vector3 mRotation;
    float mHeight = -9.0f;
    float mPower = 0.0f;
};