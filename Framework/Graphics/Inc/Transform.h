#pragma once
#include"Common.h"

using namespace GGEngine::GGMath;

namespace GGEngine::Graphic
{
    struct Transform
    {
        Vector3 position = Vector3::Zero;
        Quaternion rotation = Quaternion::Identity;
        Vector3 scale = Vector3::One;

        Transform() = default;
        Transform(const Matrix4& m)
        {
            position = GetTranslation(m);
            rotation = Quaternion::CreateFromRotationMatrix(m);
            scale = GetScale(m);
        }

        Matrix4 GetMatrix4()const
        {
            return{
            Matrix4::Scaling(scale) *
            Matrix4::MatrixRotationQuaternion(rotation) *
            Matrix4::Translation(position)
            };
        }
    };
}
