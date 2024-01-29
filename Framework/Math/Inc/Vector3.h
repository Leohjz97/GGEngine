#pragma once

namespace GGEngine::GGMath
{
	struct Vector3
	{
		union
		{
			struct
			{
				float x, y, z;
			};
			std::array<float, 3>v;
		};

		constexpr Vector3() :Vector3(0.0f) {}
		explicit constexpr Vector3(float n) noexcept : Vector3(n, n, n) {}
		constexpr Vector3(float x, float y, float z) : x{ x }, y{ y }, z{ z } {}

		static const Vector3 Zero;
		static const Vector3 One;
		static const Vector3 XAxis;
		static const Vector3 YAxis;
		static const Vector3 ZAxis;

		constexpr bool operator==(const Vector3 v) const { return x == v.x && y == v.y && z == v.z; }
		constexpr Vector3 operator-() const { return{ -x,-y,-z }; }
		constexpr Vector3 operator+(const Vector3 v) const { return{ x + v.x, y + v.y, z + v.z }; }
		constexpr Vector3 operator-(const Vector3 v) const { return{ x - v.x, y - v.y, z - v.z }; }
		constexpr Vector3 operator*(float s) const { return{ x * s, y * s , z * s }; }
		constexpr Vector3 operator/(float s) const
		{
			const float rs = 1 / s;
			return { x * rs,y * rs, z * rs };
		}

		Vector3& operator+=(const Vector3& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector3& operator-=(const Vector3& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		Vector3& operator*=(float s)
		{
			x *= s;
			y *= s;
			z *= s;
			return *this;
		}

		Vector3& operator/=(float s)
		{
			float rs = 1 / s;
			x *= rs;
			y *= rs;
			z *= rs;

			return *this;
		}
	};
}