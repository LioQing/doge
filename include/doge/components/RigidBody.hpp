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
        float density = 0.f;
        float restitution = 0.f;
        float friction = 0.2f;
    };
}