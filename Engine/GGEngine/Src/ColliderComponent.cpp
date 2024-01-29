#include "Precompiled.h"
#include "GameObject.h"
#include "ColliderComponent.h"

using namespace GGEngine;
using namespace GGEngine::Physics;

void ColliderComponent::Initialize()
{
    ASSERT(mCollisionShape != nullptr, "ColliderComponent: is NOT Set");
}

void ColliderComponent::Terminate()
{
    mCollisionShape->Terminate();
    delete mCollisionShape;
}

void ColliderComponent::SetSphereCollider(float radius)
{
    ASSERT(mCollisionShape == nullptr, "ColliderComponent: is already Set");
    auto* sphere = new CollisionShapeSphere();
    sphere->Initialize(radius);
    mCollisionShape = sphere;

}

void ColliderComponent::SetBoxCollider(const GGMath::Vector3& halfExtents)
{
    ASSERT(mCollisionShape == nullptr, "ColliderComponent: is already Set");
    auto* box = new CollisionShapeBox();
    box->Initialize(halfExtents);
    mCollisionShape = box;
}

void ColliderComponent::SetHullCollider(const GGMath::Vector3& halfExtents, const GGMath::Vector3& origin)
{
    ASSERT(mCollisionShape == nullptr, "ColliderComponent: is already Set");
    auto* hull = new CollisionShapeHull();
    hull->Initialize(halfExtents, origin);
    mCollisionShape = hull;
}

void ColliderComponent::ParseColliderComponent(const rj::Value& data, GameObject& obj)
{
	auto* colliderComponent = obj.AddComponent<ColliderComponent>();

	if (data.HasMember("Shape"))
	{
		const auto& shapData = data["Shape"].GetObj();

		if (shapData.HasMember("Type"))
		{
			const char* shapeType = shapData["Type"].GetString();
			if (strcmp(shapeType, "Box") == 0)
			{
				const auto& halfEx = shapData["HalfExtents"].GetArray();
				const float x = halfEx[0].GetFloat();
				const float y = halfEx[1].GetFloat();
				const float z = halfEx[2].GetFloat();

				colliderComponent->SetBoxCollider({ x,y,z });
			}
			else if (strcmp(shapeType, "Sphere") == 0)
			{
				const float rads = shapData["Radius"].GetFloat();
				colliderComponent->SetSphereCollider(rads);
			}
			else if (strcmp(shapeType, "Hull") == 0)
			{
				const auto& halfEx = shapData["HalfExtents"].GetArray();
				const auto& origin = shapData["Origin"].GetArray();
				const float ex = halfEx[0].GetFloat();
				const float ey = halfEx[1].GetFloat();
				const float ez = halfEx[2].GetFloat();
				const float ox = origin[0].GetFloat();
				const float oy = origin[1].GetFloat();
				const float oz = origin[2].GetFloat();

				colliderComponent->SetHullCollider({ ex,ey,ez }, { ox,oy,oz });
			}
			else
			{
				ASSERT(false, "ShapeType: %s is not in the library", shapeType);
			}
		}
		else
		{
			ASSERT(false, "ColliderComponent: shapeTYpe not available");
		}
	}
}
