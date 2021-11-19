#pragma once

#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct CircleShape
    {
        float radius = 1.f;
        Vec2f position = Vec2f::Zero();
        Vec2f scale = Vec2f::One();
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
        size_t point_count = 32;
    };
}