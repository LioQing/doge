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
        return (point - position).Magnitude() <= radius;
    }

    bool math::TestPoint(const Vec2f& point, const Rectf& rectangle)
    {
        return 
            point.x >= rectangle.left &&
            point.y >= rectangle.top &&
            point.x <= rectangle.left + rectangle.width &&
            point.y <= rectangle.top + rectangle.height;
    }

    bool math::TestPoint(const Vec2f& point, const Rectf& rectangle, float corner_radius)
    {
        if (!(
            point.x >= rectangle.left &&
            point.y >= rectangle.top &&
            point.x <= rectangle.left + rectangle.width &&
            point.y <= rectangle.top + rectangle.height
        ))
        {
            return false;
        }
        else if (corner_radius <= 0)
        {
            return true;
        }
        else if (rectangle.width == rectangle.height && corner_radius >= rectangle.width)
        {
            return math::TestPoint(point, rectangle.GetPosition() + rectangle.GetSize() / 2.f, corner_radius);
        }

        if (TestPoint(point, Rectf(rectangle.GetPosition(), corner_radius * Vec2f::One)))
        {
            return (point - (rectangle.GetPosition() + corner_radius * Vec2f::One)).Magnitude() <= corner_radius;
        }
        else if (TestPoint(point, Rectf(rectangle.GetPosition() + (rectangle.GetSize().x - corner_radius) * Vec2f::i, corner_radius * Vec2f::One)))
        {
            return (point - (rectangle.GetPosition() + Vec2f(rectangle.GetSize().x - corner_radius, corner_radius))).Magnitude() <= corner_radius;
        }
        else if (TestPoint(point, Rectf(rectangle.GetPosition() + rectangle.GetSize() - corner_radius * Vec2f::One, corner_radius * Vec2f::One)))
        {
            return (point - (rectangle.GetPosition() + rectangle.GetSize() - corner_radius * Vec2f::One)).Magnitude() <= corner_radius;
        }
        else if (TestPoint(point, Rectf(rectangle.GetPosition() + (rectangle.GetSize().y - corner_radius) * Vec2f::j, corner_radius * Vec2f::One)))
        {
            return (point - (rectangle.GetPosition() + Vec2f(corner_radius, rectangle.GetSize().y - corner_radius))).Magnitude() <= corner_radius;
        }

        return true;
    }

    bool math::TestPoint(const Vec2f& point, const std::vector<Vec2f>& convex)
    {
        int orientation = point.Orientation(convex.at(0), convex.at(1));
        for (std::size_t i = 1; i < convex.size(); ++i)
        {
            auto orient = point.Orientation(convex.at(i), convex.at((i + 1) % convex.size()));
            if (orient != orientation && orient != 0)
                return false;
        }
        return true;
    }
}