#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct Position
    {
        Position() = default;
        Position(const Position&) = default;
        Position(const Vec2f& position);
        Position(float x, float y);

        Vec2f position = Vec2f::Zero;
    };
}