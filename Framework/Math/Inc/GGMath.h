#pragma once
#include "Common.h"
#include "Constants.h"
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"

namespace GGEngine::GGMath
{
	template <class T>
	constexpr T Min(T a, T b)
	{
		return (a < b) ? a : b;
	}

	template <class T>
	constexpr T Max(T a, T b)
	{
		return (a > b) ? a : b;
	}

	template <class T>
	constexpr T Clamp(T value, T min, T max)
	{
		return Max(min, Min(max, value));
	}

	template <class T>
	constexpr T Lerp(T a, T b, float t)
	{
		return a + (b - a) * t;
	}

	template <class T>
	constexpr T Abs(T value)
	{
		return value >= 0 ? value : -value;
	}

	template <class T>
	constexpr T Sqr(T value)
	{
		return value * value;
	}

	//Vector2
	static inline float Dot(const Vector2& v1, const Vector2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	static inline float DistanceSqr(const Vector2& v1, const Vector2& v2)
	{
		return(((v1.x - v2.x) * (v1.x - v2.x)) +
			((v1.y - v2.y) * (v1.y - v2.y)));
	}

	//Vector3
	static inline float Dot(const Vector3& v1, const Vector3& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	static inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
	{
		return Vector3((v1.y * v2.z - v1.z * v2.y), (v1.z * v2.x - v1.x * v2.z), (v1.x * v2.y - v1.y * v2.x));
	}

	inline float MagnitudeSqr(const Vector3& v) { return Dot(v, v); }

	inline float Magnitude(const Vector3& v) { return sqrt(MagnitudeSqr(v)); }

	inline Vector3 Normalize(const Vector3& v) {
		float m = sqrt(abs(v.x * v.x + v.y * v.y + v.z * v.z));
		return v / m;
	}
	static inline float Distance(const Vector3& v1, const Vector3& v2)
	{
		return (sqrt(
		((v1.x - v2.x) * (v1.x - v2.x)) +
		((v1.y - v2.y) * (v1.y - v2.y)) +
		((v1.z - v2.z) * (v1.z - v2.z))
		));
	}
	static inline float DistanceSqr(const Vector3& v1, const Vector3& v2)
	{
		return (
			((v1.x - v2.x) * (v1.x - v2.x)) +
			((v1.y - v2.y) * (v1.y - v2.y)) +
			((v1.z - v2.z) * (v1.z - v2.z))
		);
	}

	inline Vector3 TransformCoord(const Vector3& v, const Matrix4& m)
	{
		float x = v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41;
		float y = v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42;
		float z = v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43;

		return { x, y, z };
	}

	inline Vector3 TransformNormal(const Vector3& v, const Matrix4& m)
	{
		float x = v.x * m._11 + v.y * m._21 + v.z * m._31;
		float y = v.x * m._12 + v.y * m._22 + v.z * m._32;
		float z = v.x * m._13 + v.y * m._23 + v.z * m._33;

		return { x, y, z };
	}

	//Vector4
	static inline float Dot(const Vector4& v1, const Vector4& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	static inline float Cross(Vector4& v1, Vector4& v2)
	{
		return (
			((v1.y - v2.z) - (v1.z - v2.y)) +
			((v1.z - v2.x) * (v1.x - v2.z)) +
			((v1.x - v2.y) * (v1.y - v2.x)) +
			((v1.w - v2.w) * (v1.w - v2.w))
		);
	}
	static inline float Distance(Vector4& v1, Vector4& v2)
	{
		return (sqrt(
			((v1.x - v2.x) * (v1.x - v2.x)) +
			((v1.y - v2.y) * (v1.y - v2.y)) +
			((v1.z - v2.z) * (v1.z - v2.z)) +
			((v1.w - v2.w) * (v1.w - v2.w))
		));
	}
	static inline float DistanceSqr(Vector4& v1, Vector4& v2)
	{
		return (
			((v1.x - v2.x) * (v1.x - v2.x)) +
			((v1.y - v2.y) * (v1.y - v2.y)) +
			((v1.z - v2.z) * (v1.z - v2.z)) +
			((v1.w - v2.w) * (v1.w - v2.w))
		);
	}

	//Matrix4
	inline Matrix4 Transpose(const Matrix4& m)
	{
		return Matrix4(
			m._11, m._21, m._31, m._41,
			m._12, m._22, m._32, m._42,
			m._13, m._23, m._33, m._43,
			m._14, m._24, m._34, m._44
		);
	}

	inline Matrix4 RotationQuaternion(const Quaternion& q)
	{
		float n = 1.0f / sqrtf(q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z);
		float s = q.w * n;
		float x = q.x * n;
		float y = q.y * n;
		float z = q.z * n;
		Matrix4 m{ 1 - 2 * y * y - 2 * z * z,2 * x * y - 2 * s * z,2 * x * z + 2 * s * y,0.0f,
				  2 * x * y + 2 * s * z, 1 - 2 * x * x - 2 * z * z,2 * y * z - 2 * s * x,0.0f,
				  2 * x * z - 2 * s * y,2 * y * z + 2 * s * x,1 - 2 * x * x - 2 * y * y ,0.0f,
				   0.0f,0.0f,0.0f,1.0f };
		m = Transpose(m);
		return m;
	}

	inline Matrix4 Matrix4::RotationAxis(const Vector3& axis, float rad)
	{
		const Vector3 u = Normalize(axis);
		const float x = u.x;
		const float y = u.y;
		const float z = u.z;
		const float s = sin(rad);
		const float c = cos(rad);

		return {
			c + (x * x * (1.0f - c)),
			x * y * (1.0f - c) + z * s,
			x * z * (1.0f - c) - y * s,
			0.0f,

			x * y * (1.0f - c) - z * s,
			c + (y * y * (1.0f - c)),
			y * z * (1.0f - c) + x * s,
			0.0f,

			x * z * (1.0f - c) + y * s,
			y * z * (1.0f - c) - x * s,
			c + (z * z * (1.0f - c)),
			0.0f,

			0.0f, 0.0f, 0.0f, 1.0f
		};
	}

	inline Vector3 GetRight(const Matrix4& m)
	{
		return { m._11,m._12, m._13 };
	}

	inline Vector3 GetUp(const Matrix4& m)
	{
		return { m._21,m._22, m._23 };
	}

	inline Vector3 GetForward(const Matrix4& m)
	{
		return { m._31,m._32, m._33 };
	}
	
	inline Vector3 GetTranslation(const Matrix4& m)
	{
		return { m._41,m._42, m._43 };
	}

	inline Vector3 GetScale(const Matrix4& m)
	{
		return { m._11,m._22, m._33 };
	}
	
	inline Vector3 GetLookAt(const Matrix4& m)
	{
		return { m._31, m._32, m._33 };
	}
}