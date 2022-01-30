#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/aliases.hpp>

namespace doge::physics
{
    struct CircleCollider
    {
        EntityID rigid_body_entity;

        float radius = 1.f;
        Vec2f origin = Vec2f::Zero;
        
        float density = 1.f;
        float friction = 0.2f;
        float restitution = 0.f;
        float restitution_threshold = 1.f;
        
        bool is_trigger = false;

        // doge::Physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;
    };
}