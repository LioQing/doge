#include <doge/utils/math.hpp>

#include <numbers>
#include <cmath>

namespace doge
{
    float math::ConstrainAngle(float rad)
    {
        using std::numbers::pi;

        rad = std::fmod(rad + pi, pi * 2.f);
        if (rad < 0)
            rad += pi * 2.f;
        return rad - pi;
    }

    bool math::TestPoint(const Vec2f& point, const Vec2f& position, float radius)
    {
        return (point - position).Magnitude() < radius;
    }

    bool math::TestPoint(const Vec2f& point, const Rectf& rectangle)
    {
        return 
            point.x > rectangle.left &&
            point.y > rectangle.top &&
            point.x < rectangle.left + rectangle.width &&
            point.y < rectangle.top + rectangle.height;
    }

    bool math::TestPoint(const Vec2f& point, const std::vector<Vec2f>& convex)
    {
        int orientation = point.Orientation(convex.at(0), convex.at(1));
        for (std::size_t i = 1; i < convex.size(); ++i)
        {
            if (point.Orientation(convex.at(i), convex.at((i + 1) % convex.size())) != orientation)
                return false;
        }
        return true;
    }
}