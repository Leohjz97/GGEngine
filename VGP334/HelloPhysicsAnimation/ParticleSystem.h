#pragma once
#include <GGEngine/Inc/GGEngine.h>

using namespace GGEngine::GGMath;
using namespace GGEngine::Graphic;
using namespace GGEngine::Physics;


struct Info
{
    float life;
    float duration;
    GGEngine::Color startColor = GGEngine::Colors::White;
    GGEngine::Color endColor = GGEngine::Colors::Red;
    Vector3 startPos = Vector3::Zero;
    Vector3 startVel = Vector3::Zero;
};

class ParticleSystem
{
public: 
    ParticleSystem() = default;

    void Initialize(const Info& info);
    void Terminate();

    void Update(float deltTime);
    void Render();

private:
    RenderObject mParticle;
    RigidBody rb;
    CollisionShapeSphere mSphere;

    float mDuration = 0.0f;
    float mLife = 0.0f;
    GGEngine::Color startColor = GGEngine::Colors::White;
    GGEngine::Color endColor = GGEngine::Colors::Red;
};