#pragma once

#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <vector>

namespace doge
{
    struct math
    {
        math(const math&) = delete;

        static float ConstrainAngle(float rad);

        static bool TestPoint(const Vec2f& point, const Vec2f& position, float radius);
        static bool TestPoint(const Vec2f& point, const Rectf& rectangle);
        static bool TestPoint(const Vec2f& point, const std::vector<Vec2f>& convex);
    };
}