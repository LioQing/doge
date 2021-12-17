#pragma once

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
    };
}