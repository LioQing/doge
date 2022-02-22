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
        static bool TestPoint(const Vec2f& point, const Rectf& rectangle, float corner_radius);
        static bool TestPoint(const Vec2f& point, const std::vector<Vec2f>& convex);

        template <typename T, typename U>
        static Vec2<T> AutoSize(const Vec2<T>& size, const Vec2<U>& auto_size)
        {
            auto new_size = size;

            if (new_size.x == 0)
                new_size.x = auto_size.x;

            if (new_size.y == 0)
                new_size.y = auto_size.y;
            
            return new_size;
        }

        template <typename T, typename U>
        static Rect<T> AutoSize(const Rect<T>& rect, const Vec2<U>& auto_size)
        {
            auto new_rect = rect;

            if (new_rect.width == 0.f)
                new_rect.width = auto_size.x;

            if (new_rect.height == 0.f)
                new_rect.height = auto_size.y;
            
            return new_rect;
        }
    };
}