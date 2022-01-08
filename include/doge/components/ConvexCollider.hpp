#pragma once

#include <vector>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct ConvexCollider
    {
        // collision only happen on right side of edge
        std::vector<Vec2f> points;

        Vec2f origin = Vec2f::Zero();
        
        float density = 1.f;
        float restitution = 0.f;
        float friction = 0.2f;
        
        // doge::physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;
    };
}