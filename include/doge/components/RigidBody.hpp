#pragma once

#include <box2d/box2d.h>

namespace doge
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
    };
}