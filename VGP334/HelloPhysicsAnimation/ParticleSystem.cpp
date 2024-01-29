#include "ParticleSystem.h"

using namespace GGEngine;
using namespace GGEngine::Graphic;

void ParticleSystem::Initialize(const Info& info)
{
    mLife = 0;
    mDuration = info.duration;
    startColor = info.startColor;
    endColor = info.endColor;

    MeshPC mesh = MeshBuilder::CreateSpherePC(10.0f, 10.0f, 0.15f);
    for (auto& v : mesh.verticies)
    {
        v.color = startColor;
    }

    mParticle.meshBuffer.Initialize(mesh);
    mParticle.trasf.position = info.startPos;

    mSphere.Initialize(0.15f);
    rb.Initialize(mParticle.trasf, mSphere, 0.1f);
    rb.SetCollisionFilter(0);
    rb.SetVelocity(info.startVel);
}

void ParticleSystem::Terminate()
{
    rb.Terminate();
    mSphere.Terminate();
    mParticle.Terminate();
}

void ParticleSystem::Update(float deltTime)
{
    Color particlecolor = Lerp(startColor, endColor, Clamp(mLife / mDuration, 0.0f, 1.0f));

}

void ParticleSystem::Render()
{
}
