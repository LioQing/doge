#pragma once

#include <vector>
#include <doge/components/ConvexShape.hpp>
#include <doge/components/CircleShape.hpp>
#include <doge/components/RectangleShape.hpp>
#include <doge/components/Sprite.hpp>

namespace doge
{
    struct CompoundSprite
    {
        std::vector<ConvexShape> convex_shapes;
        std::vector<CircleShape> circle_shapes;
        std::vector<RectangleShape> rectangle_shapes;
        std::vector<Sprite> sprites;
    };
}