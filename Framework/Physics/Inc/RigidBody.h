#pragma once
namespace GGEngine::Graphic
{
    struct Transform;
}
namespace GGEngine::Physics
{
    class CollisionShape;

    class RigidBody final
    {
    public:
        RigidBody() = default;
        ~RigidBody();

        void Initialize(Graphic::Transform& gTransform, const CollisionShape& shape, float mass = 0.0f);
        void Terminate();
        
        void SetCollisionFilter(int flag);
        void SetPosition(const GGEngine::GGMath::Vector3& pos);
        void SetVelocity(const GGEngine::GGMath::Vector3& vel);

        bool IsDynamic()const;
        void UpdateTransform();

    private:
        friend class PhysicsWorld;
        btRigidBody* GetRigidBody() { return rb; }

        btRigidBody* rb = nullptr;
        btDefaultMotionState* mMotionState = nullptr;
        float mMass = 0.0f;
        bool mIsDynamic;
        Graphic::Transform* mGraphicTransform = nullptr;
    };
}