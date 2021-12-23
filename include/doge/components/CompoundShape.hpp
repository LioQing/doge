#pragma once

#include <vector>
#include <doge/components/ConvexShape.hpp>
#include <doge/components/CircleShape.hpp>
#include <doge/components/RectangleShape.hpp>

namespace doge
{
    struct CompoundShape
    {
        std::vector<ConvexShape> convex_shapes;
        std::vector<CircleShape> circle_shapes;
        std::vector<RectangleShape> rectangle_shapes;
    };
}