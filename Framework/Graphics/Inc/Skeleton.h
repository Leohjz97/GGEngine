#pragma once
#include "Bone.h"

namespace GGEngine::Graphic
{
    struct Skeleton {
        Bone* root = nullptr;
        std::vector<std::unique_ptr<Bone>> bones;
    };
}