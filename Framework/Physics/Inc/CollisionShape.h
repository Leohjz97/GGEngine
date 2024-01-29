#pragma once

namespace GGEngine::Physics
{
    class CollisionShape
    {
    public:
        CollisionShape() = default;
        virtual ~CollisionShape();

        virtual void Terminate();

    protected:
        btCollisionShape* shape = nullptr;

    private:
        friend class RigidBody;
        btCollisionShape* GetCollisionShape() const { return shape; }
    };

    class CollisionShapeSphere : public CollisionShape
    {
    public:
        CollisionShapeSphere() = default;
        void Initialize(float r);
    };

    class CollisionShapeBox : public CollisionShape
    {
    public:
        CollisionShapeBox() = default;
        void Initialize(const GGEngine::GGMath::Vector3& extend);
    };

    class CollisionShapeHull: public CollisionShape
    {
    public:
        CollisionShapeHull() = default;
        void Initialize(const GGEngine::GGMath::Vector3& halfextend, const GGEngine::GGMath::Vector3& origin);
    };
}
