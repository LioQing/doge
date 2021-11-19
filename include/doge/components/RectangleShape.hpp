#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    struct RectangleShape
    {
        Vec2f size = Vec2f::One();
        Vec2f position = Vec2f::Zero();
        float rotation = 0.f; // in radian
        Vec2f scale = Vec2f::One();
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
    };
}