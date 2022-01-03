#include <doge/components/ConvexShape.hpp>

namespace doge
{
    ConvexShape::ConvexShape(
            const std::vector<Vec2f>& points,
            const Vec2f& origin,
            const Color& color
        ) :
        points(points),
        origin(origin),
        color(color)
    {
    }

    std::vector<Vec2f>& ConvexShape::GetPoints()
    {
        is_modified = true;
        return points;
    }

    const std::vector<Vec2f>& ConvexShape::GetPoints() const
    {
        return points;
    }

    Vec2f& ConvexShape::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& ConvexShape::GetOrigin() const
    {
        return origin;
    }

    Color& ConvexShape::GetColor()
    {
        is_modified = true;
        return color;
    }

    const Color& ConvexShape::GetColor() const
    {
        return color;    
    }

    std::vector<Vec2f>& ConvexShape::_NoModify_GetPoints()
    {
        return points;
    }

    Vec2f& ConvexShape::_NoModify_GetOrigin()
    {
        return origin;
    }

    Color& ConvexShape::_NoModify_GetColor()
    {
        return color;
    }

    void ConvexShape::_ResetModify()
    {
        is_modified = false;
    }

    bool ConvexShape::_IsModified() const
    {
        return is_modified;
    }
}