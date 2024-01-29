#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsDebugDrawer.h"
#include "RigidBody.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;
using namespace GGEngine::Physics;

namespace
{
    std::unique_ptr<PhysicsWorld> phWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& sets)
{
    ASSERT(phWorld == nullptr, "PhysicsWorld is already initialized");
    phWorld = std::make_unique<PhysicsWorld>();
    phWorld->Initialize(sets);
}

void PhysicsWorld::StaticTerminate()
{
    if (phWorld!= nullptr)
    {
        phWorld->Terminate(); 
        phWorld.reset();
    }
}

PhysicsWorld* PhysicsWorld::Get()
{
    ASSERT(phWorld != nullptr, "PhysicsWorld is not initialized!");
    return phWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(mDynamicWorld == nullptr, "PhysicsWorld terminate must be called before deletion");
}

void PhysicsWorld::Initialize(const Settings& sets)
{
    mSets = sets;
    mCollisionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mInterface = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver();
    mDynamicWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);
    mDynamicWorld->setGravity(ConvertTobtVector3(sets.g));

    mDebugDrawer = new PhysicsDebugDrawer();
    mDynamicWorld->setDebugDrawer(mDebugDrawer);
}

void PhysicsWorld::Terminate()
{
    SafeDeletePhyics(mDebugDrawer);
    SafeDeletePhyics(mDynamicWorld);
    SafeDeletePhyics(mSolver);
    SafeDeletePhyics(mInterface);
    SafeDeletePhyics(mDispatcher);
    SafeDeletePhyics(mCollisionConfiguration);
}

void PhysicsWorld::SetGravity(const GGEngine::GGMath::Vector3 g)
{
    mDynamicWorld->setGravity(ConvertTobtVector3(g));
}

void PhysicsWorld::Update(float deltaTime)
{
    mDynamicWorld->stepSimulation(deltaTime, mSets.simulation, mSets.fixedTimeStep);
    for (auto& rb: rbs)
    {
        rb->UpdateTransform();
    }
}

void PhysicsWorld::DebugUI()
{
    ImGui::Checkbox("RenderPhysics##", &mRenderDebugUI);
    if (mRenderDebugUI)
    {
        int debugMode = mDebugDrawer->getDebugMode();
        bool isEnabled = (debugMode & btIDebugDraw::DBG_DrawWireframe) > 0;
        if (ImGui::Checkbox("[DBG]DrawWireFrame##", &isEnabled))
        {
            debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;
        }

        isEnabled= (debugMode & btIDebugDraw::DBG_DrawAabb) > 0;
        if (ImGui::Checkbox("[DBG]DrawAABB##", &isEnabled))
        {
            debugMode = (isEnabled) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
        }

        mDebugDrawer->setDebugMode(debugMode);
        mDynamicWorld->debugDrawWorld();
    }
}

void PhysicsWorld::Register(RigidBody* rb)
{
    if (rb != nullptr)
    {
        rbs.push_back(rb);
        if (rb->GetRigidBody() != nullptr)
        {
            mDynamicWorld->addRigidBody(rb->GetRigidBody());
        }
    }
}

void PhysicsWorld::UnRegister(RigidBody* rb)
{
    if (rb != nullptr)
    {
        auto iter = std::find(rbs.begin(), rbs.end(), rb);
        if (iter != rbs.end())
        {
            if (rb->GetRigidBody() != nullptr)
            {
                mDynamicWorld->removeRigidBody(rb->GetRigidBody());
            }
            rbs.erase(iter);
        }
    }
}
