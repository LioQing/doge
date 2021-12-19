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
    };
}