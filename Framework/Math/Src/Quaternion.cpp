#include "Precompiled.h"
#include "../Inc/Quaternion.h"
#include "Math/Inc/GGMath.h"
#include "Math/Inc/Matrix4.h"

/****************************************************************************
 *
 * Quaternion
 *
 ****************************************************************************/

 //------------------------------------------------------------------------------
 // Comparision operators
 //------------------------------------------------------------------------------

namespace GGEngine::GGMath
{
    //------------------------------------------------------------------------------
    // Quaternion operations
    //------------------------------------------------------------------------------

    inline void Quaternion::Conjugate() noexcept
    {
        x = -x;
        y = -y;
        z = -z;
    }

    inline void Quaternion::Conjugate(Quaternion& result) const noexcept
    {
        result.Conjugate();
    }

    inline void Quaternion::Inverse(Quaternion& result) const noexcept
    {
        float magnitude = result.Magnitude(result);
        result.Conjugate();
        result = result / (magnitude + magnitude);
    }

    inline float Quaternion::Dot(const Quaternion& q) const noexcept
    {
        return x * q.x + y * q.y + z * q.z + w * q.w;
    }

    float Quaternion::Magnitude(const Quaternion& q)
    {
        return std::sqrt((q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w));
    }

    Quaternion Quaternion::Normalize(const Quaternion& q)
    {
        return q / Magnitude(q);
    }

    //------------------------------------------------------------------------------
    // Static functions
    //------------------------------------------------------------------------------

    Quaternion Quaternion::CreateFromAxisAngle(const Vector3& axis, float angle) noexcept
    {
        const float c = cos(angle * 0.5f);
        const float s = sin(angle * 0.5f);
        const GGMath::Vector3 a = GGMath::Normalize(axis);
        return Quaternion(a.x * s, a.y * s, a.z * s, c);
    }

    Quaternion Quaternion::CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept
    {
        float cosYaw = cos(yaw * 0.5f);
        float sinYaw = sin(yaw * 0.5f);
        float cosPitch = cos(pitch * 0.5f);
        float sinPitch = sin(pitch * 0.5f);
        float cosRoll = cos(roll * 0.5f);
        float sinRoll = sin(roll * 0.5f);

        Quaternion q;
        q.x = cosRoll * sinPitch * cosYaw - sinRoll * cosPitch * sinYaw;
        q.y = cosRoll * cosPitch * sinYaw + sinRoll * sinPitch * cosYaw;
        q.z = sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw;
        q.w = cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw;

        return q;
    }

    Quaternion Quaternion::CreateFromRotationMatrix(const Matrix4& M) noexcept
    {
        Quaternion q;
        q.w = sqrt(1.0f + M._11 + M._22 + M._33);

        float w4 = q.w * 4;
        q.x = (M._32 - M._23) / (w4);
        q.y = (M._12 - M._31) / (w4);
        q.z = (M._21 - M._12) / (w4);

        return q;
    }

    Quaternion Quaternion::Lerp(Quaternion q0, Quaternion q1, float t)
    {
        return q0 * (1.0f - t) + (q1 * t);
    }

    Quaternion Quaternion::Slerp(Quaternion q1, Quaternion q2, float t)
    {
        // Find the dot product
        float dot = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

        // Determine the direction of the rotation.
        if (dot < 0.0f)
        {
            dot = -dot;
            q2.x = -q2.x;
            q2.y = -q2.y;
            q2.z = -q2.z;
            q2.w = -q2.w;
        }
        if (dot > 0.999f)
        {
            return Normalize(Lerp(q1, q2, t));
        }

        float theta = acosf(dot);
        float sintheta = sinf(theta);
        float scale0 = sinf(theta * (1.0f - t)) / sintheta;
        float scale1 = sinf(theta * t) / sintheta;

        // Perform the slerp
        return Quaternion
        (
            (q1.x * scale0) + (q2.x * scale1),
            (q1.y * scale0) + (q2.y * scale1),
            (q1.z * scale0) + (q2.z * scale1),
            (q1.w * scale0) + (q2.w * scale1)
        );
    }
    Quaternion Quaternion::RotationAxis(const Vector3& axis, float rad) noexcept
    {
        Vector3 m = GGEngine::GGMath::Normalize(axis);

        return { cos(rad / 2),sin(rad / 2) * m.x,sin(rad / 2) * m.y,sin(rad / 2) * m.z };
    }
    Quaternion Quaternion::RotationEuler(const Vector3& eulerAngles) noexcept
    {
        const float cr = cos(eulerAngles.x * 0.5f);
        const float sr = sin(eulerAngles.x * 0.5f);
        const float cy = cos(eulerAngles.z * 0.5f);
        const float sy = sin(eulerAngles.z * 0.5f);
        const float cp = cos(eulerAngles.y * 0.5f);
        const float sp = sin(eulerAngles.y * 0.5f);

        return {
            cr * cp * cy + sr * sp * sy,
            sr * cp * cy - cr * sp * sy,
            cr * sp * cy + sr * cp * sy,
            cr * cp * sy - sr * sp * cy
        };
    }
    Quaternion Quaternion::RotationMatrix(const Matrix4& mat) noexcept
    {
        Matrix4 m = Transpose(mat);
        float qw, qx, qy, qz;

        float tr = m._11 + m._22 + m._33;

        if (tr > 0)
        {
            float w = sqrt(tr + 1.0f) * 2.0f;
            qw = 0.25f * w;
            qx = (m._32 - m._23) / w;
            qy = (m._13 - m._31) / w;
            qz = (m._21 - m._12) / w;
        }
        else if ((m._11 > m._22) && (m._11 > m._33))
        {
            float w = sqrt(1.0f + m._11 - m._22 - m._33) * 2.0f;
            qw = (m._32 - m._23) / w;
            qx = 0.25f * w;
            qy = (m._12 + m._21) / w;
            qz = (m._13 + m._31) / w;
        }
        else if (m._22 > m._33)
        {
            float w = sqrt(1.0f + m._22 - m._11 - m._33) * 2.0f;
            qw = (m._13 - m._31) / w;
            qx = (m._12 + m._21) / w;
            qy = 0.25f * w;
            qz = (m._23 + m._32) / w;
        }
        else
        {
            float w = sqrt(1.0f + m._33 - m._11 - m._22) * 2.0f;
            qw = (m._21 - m._12) / w;
            qx = (m._13 + m._31) / w;
            qy = (m._23 + m._32) / w;
            qz = 0.25f * w;
        }
        return Normalize(Quaternion(qw, qx, qy, qz));
    }
    Quaternion Quaternion::RotationLook(const Vector3& direction, const Vector3& up) noexcept
    {
        Vector3 L =  GGEngine::GGMath::Normalize(direction);
        Vector3 R =  GGEngine::GGMath::Normalize(Cross(up, L));
        Vector3 U =  GGEngine::GGMath::Normalize(Cross(L, R));

        Matrix4 m{ R.x,R.y,R.z,0.0f,
                  U.x,U.y,U.z,0.0f,
                  L.x,L.y,L.z,0.0f,
                  0.0f,0.0f,0.0f,1.0f };

        Quaternion q = RotationMatrix(m);
        return q;
    }
    Quaternion Quaternion::RotationFromTo(const Vector3& from, const Vector3& to) noexcept
    {
        Vector3 f =  GGEngine::GGMath::Normalize(from);
        Vector3 t =  GGEngine::GGMath::Normalize(to);
        Vector3 axis = Cross(f, t);
        float theta = acos(GGEngine::GGMath::Dot(f, t));
        Quaternion q = RotationAxis(axis, theta);
        Normalize(q);
        return q;
    }
}