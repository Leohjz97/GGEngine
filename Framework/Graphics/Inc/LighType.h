#pragma once
#include "Colors.h"

using namespace GGEngine;
using namespace GGEngine::GGMath;

namespace GGEngine::Graphic
{
    struct DirectionaLight
    {
        Vector3 direction = Vector3::Zero;
        float padding = 0.0f;
        Color ambient = Colors::White;
        Color diffuse = Colors::White;
        Color sepcular = Colors::White;
    };
}