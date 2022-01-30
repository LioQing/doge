#pragma once

#include <vector>
#include <doge/utils/Vec2.hpp>

namespace doge::physics
{
    struct EdgeCollider
    {
        EntityID rigid_body_entity;

        // collision only happen on left side of edge
        std::vector<Vec2f> points;

        bool is_loop = false;
        Vec2f origin = Vec2f::Zero;
        
        float density = 0.f;
        float friction = 0.2f;
        float restitution = 0.f;
        float restitution_threshold = 1.f;
        
        bool is_trigger = false;
        
        // doge::Physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;
    };
}