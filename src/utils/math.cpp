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

    std::vector<Vec2f> math::RoundedRectangle(const Vec2f& size, float radius, std::uint32_t corner_vertex_count)
    {
        using std::numbers::pi;

        if (corner_vertex_count == 1)
            return std::vector<Vec2f>{ size, Vec2f(size.x, 0), Vec2f::Zero(), Vec2f(0, size.y) };

        std::vector<Vec2f> vertices(corner_vertex_count * 4);

        for (auto i = 0; i < corner_vertex_count; ++i)
        {
            auto theta = pi * (corner_vertex_count - i - 1) / 2.f / (corner_vertex_count - 1);

            vertices.at(i)                           = (Vec2f::Right() * radius).Rotated(theta)                   + size - Vec2f(radius, radius);
            vertices.at(corner_vertex_count * 1 + i) = (Vec2f::Right() * radius).Rotated(theta - pi / 2.f)        + Vec2f(size.x - radius, radius);
            vertices.at(corner_vertex_count * 2 + i) = (Vec2f::Right() * radius).Rotated(theta - pi)              + Vec2f(radius, radius);
            vertices.at(corner_vertex_count * 3 + i) = (Vec2f::Right() * radius).Rotated(theta - pi * 3.f / 2.f)  + Vec2f(radius, size.y - radius);
        }

        return vertices;
    }
}