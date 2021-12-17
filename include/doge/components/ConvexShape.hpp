#pragma once

#include <vector>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    struct ConvexShape
    {
        std::vector<Vec2f> points;
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
    };
}