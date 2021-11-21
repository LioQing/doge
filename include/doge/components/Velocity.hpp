#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct Velocity
    {
        Velocity() = default;
        Velocity(const Velocity&) = default;
        Velocity(const Vec2f& velocity);
        Velocity(float x, float y);

        Vec2f velocity = Vec2f::Zero();
    };
}