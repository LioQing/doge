#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct PhysicsBodyInit
    {
        Vec2f position = Vec2f::Zero();
        float rotation = 0.f;

        Vec2f velocity = Vec2f::Zero();
        float angular_velocity = 0.f;

        float damping = 0.f;
        float angular_damping = 0.f;
    };
}