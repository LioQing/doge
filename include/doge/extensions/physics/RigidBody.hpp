#pragma once

#include <box2d/box2d.h>
#include <doge/utils/lev.hpp>

namespace doge::physics
{
    struct RigidBody
    {
        enum Type
        {
            Static,
            Kinematic,
            Dynamic,
        };

        Type type = Type::Static;

        bool rotation_fixed = false;
        bool continuous = false;
        bool awake = true;
        bool enabled = true;

        Event<Entity> on_collision_began;
        Event<Entity> on_collision_ended;
        Event<Entity> on_collision_presolve;
        Event<Entity> on_collision_postsolve;
    };
}