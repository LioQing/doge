#pragma once

#include <vector>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/aliases.hpp>

namespace doge::physics
{
    struct ConvexCollider
    {
        EntityID rigid_body_entity;

        // collision only happen on right side of edge
        std::vector<Vec2f> vertices;

        Vec2f origin = Vec2f::Zero;
        
        float density = 1.f;
        float friction = 0.2f;
        float restitution = 0.f;
        float restitution_threshold = 1.f;
        
        bool is_trigger = false;
        std::uint16_t category_bits = 0x0001;
        std::uint16_t mask_bits = 0xFFFF;
        std::int16_t group_index = 0;
        
        // doge::Physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;
    };
}