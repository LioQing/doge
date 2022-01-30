#pragma once

#include <box2d/box2d.h>

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

        bool continuous = false;
        bool awake = true;
        bool enabled = true;
    };
}