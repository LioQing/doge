#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct RectangleCollider
    {
        Vec2f size = Vec2f::One();
        Vec2f origin = Vec2f::Zero();
    };
}