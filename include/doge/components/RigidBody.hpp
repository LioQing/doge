#pragma once

#include <box2d/box2d.h>

namespace doge
{
    struct RigidBody
    {
        enum class Type
        {
            Static,
            Kinematic,
            Dynamic,
        };

        Type type = Type::Static;
        b2Body* body_ptr = nullptr;
    };
}