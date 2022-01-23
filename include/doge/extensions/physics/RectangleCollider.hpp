#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct RectangleCollider
    {
        EntityID rigid_body_entity;

        Vec2f size = Vec2f::One();
        Vec2f origin = Vec2f::Zero();
        
        float density = 1.f;
        float friction = 0.2f;
        float restitution = 0.f;
        float restitution_threshold = 1.f;
        
        bool is_trigger = false;
        
        // doge::physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;
    };
}