#pragma once
using namespace GGEngine::GGMath;
namespace GGEngine::Physics
{
    class PhysicsDebugDrawer;
    class RigidBody;

    class PhysicsWorld
    {
    public:
        struct Settings
        {
            Vector3 g{ 0.0f,-9.8f,0.0f };
            uint32_t simulation = 1;
            float fixedTimeStep = 1.0f / 60.0f;
        };

        static void StaticInitialize(const Settings& sets);
        static void StaticTerminate();
        static PhysicsWorld* Get();

        PhysicsWorld() = default;
        ~PhysicsWorld();

        void Initialize(const Settings& sets);
        void Terminate();

        void SetGravity(const GGEngine::GGMath::Vector3 g);

        void Update(float deltaTime);
        void DebugUI();

    private:
        friend class RigidBody;
        void Register(RigidBody* rb);
        void UnRegister(RigidBody* rb);
        Settings mSets;

        btBroadphaseInterface* mInterface = nullptr;
        btCollisionDispatcher* mDispatcher = nullptr;
        btDiscreteDynamicsWorld* mDynamicWorld = nullptr;
        btSequentialImpulseConstraintSolver* mSolver = nullptr;
        btDefaultCollisionConfiguration* mCollisionConfiguration = nullptr;
        PhysicsDebugDrawer* mDebugDrawer = nullptr;

        using RigidBodies = std::vector<RigidBody*>;
        RigidBodies rbs;

        bool mRenderDebugUI = false;
    };
}