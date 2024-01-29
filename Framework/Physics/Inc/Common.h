#pragma once
// FrameWork Headers
#include <Math/Inc/GGMath.h>
#include <Core/Inc/Core.h>
#include <Graphics/Inc/Transform.h>
#include <Graphics/Inc/Colors.h>

// Bullets 
#include <Bullet/btBulletCollisionCommon.h>
#include <Bullet/btBulletDynamicsCommon.h>

//Physics
template<class T>
inline void SafeDeletePhyics(T*& ptr)
{
	if (ptr)
	{
		delete ptr;
		ptr = nullptr;
	}
}

inline btVector3 ConvertTobtVector3(const GGEngine::GGMath::Vector3& vec)
{
	return { vec.x,vec.y,vec.z };
}

inline GGEngine::GGMath::Vector3 ConvertToVector3(const btVector3& vec)
{
	return { vec.x(),vec.y(),vec.z()};
}

inline btQuaternion ConvertTobtQuaternion(const GGEngine::GGMath::Quaternion& q)
{
	return { q.x,q.y,q.z,q.w };
}

inline GGEngine::GGMath::Quaternion ConvertToQuaternion(const btQuaternion& q)
{
	return { q.x(),q.y(),q.z(),q.w() };
}

inline GGEngine::Color ConvertToColor(const btVector3& btColor)
{
	return { btColor.x(),btColor.y(), btColor.z(),1.0f };
}

inline btTransform ConvertTobtTransform(const GGEngine::Graphic::Transform& tf)
{
	return btTransform(ConvertTobtQuaternion(tf.rotation),
		ConvertTobtVector3(tf.position));
}

inline GGEngine::Graphic::Transform ConverToTransform(const btTransform& ts)
{
	const auto& orign = ts.getOrigin();
	const auto& rotation = ts.getRotation();
	GGEngine::Graphic::Transform retTrans;
	retTrans.position = ConvertToVector3(orign);
	retTrans.rotation = ConvertToQuaternion(rotation);
	return retTrans;
}

inline void ApplybtTransformToTransform(const btTransform& btf, GGEngine::Graphic::Transform& tf)
{
	tf.position = ConvertToVector3(btf.getOrigin());
	tf.rotation = ConvertToQuaternion(btf.getRotation());
}