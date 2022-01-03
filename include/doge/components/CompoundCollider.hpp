#pragma once

#include <vector>
#include <doge/components/ConvexCollider.hpp>
#include <doge/components/CircleCollider.hpp>
#include <doge/components/RectangleCollider.hpp>

namespace doge
{
    struct CompoundCollider
    {
        std::vector<ConvexCollider> convex_colliders;
        std::vector<CircleCollider> circle_colliders;
        std::vector<RectangleCollider> rectangle_colliders;
    };
}