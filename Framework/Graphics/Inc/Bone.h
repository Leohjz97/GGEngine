#pragma once
#include "Common.h"

using namespace GGEngine::GGMath;

namespace GGEngine::Graphic
{
    struct Bone {
        std::string name;
        int idx = -1;

        Bone* parent = nullptr;
        int parentIdx = -1;

        std::vector<Bone*> children;
        std::vector<int> childrenIndices;

        Matrix4 toParentTransform;
        Matrix4 offsetTransform;
    };

}