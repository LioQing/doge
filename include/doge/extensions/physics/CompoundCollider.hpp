#pragma once

#include <vector>
#include <doge/extensions/physics/ConvexCollider.hpp>
#include <doge/extensions/physics/CircleCollider.hpp>
#include <doge/extensions/physics/RectangleCollider.hpp>

namespace doge
{
    struct CompoundCollider
    {
        std::vector<ConvexCollider> convex_colliders;
        std::vector<CircleCollider> circle_colliders;
        std::vector<RectangleCollider> rectangle_colliders;
        
        bool is_enabled = true;
        bool is_trigger = false;
        
        // doge::physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;
    };
}