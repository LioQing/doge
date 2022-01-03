#include <doge/components/CircleShape.hpp>

namespace doge
{
    CircleShape::CircleShape(
            float radius,
            const Vec2f& origin,
            const Color& color,
            std::size_t point_count
        ) : 
        radius(radius), 
        origin(origin), 
        color(color),
        point_count(point_count)
    {
    }

    float& CircleShape::GetRadius()
    {
        is_modified = true;
        return radius;
    }

    float CircleShape::GetRadius() const
    {
        return radius;    
    }

    Vec2f& CircleShape::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& CircleShape::GetOrigin() const
    {
        return origin;    
    }

    Color& CircleShape::GetColor()
    {
        is_modified = true;
        return color;
    }

    const Color& CircleShape::GetColor() const
    {
        return color;    
    }

    std::size_t& CircleShape::GetPointCount()
    {
        is_modified = true;
        return point_count;
    }

    std::size_t CircleShape::GetPointCount() const
    {
        return point_count;    
    }

    float& CircleShape::_NoModify_GetRadius()
    {
        return radius;
    }

    Vec2f& CircleShape::_NoModify_GetOrigin()
    {
        return origin;
    }

    Color& CircleShape::_NoModify_GetColor()
    {
        return color;
    }

    std::size_t& CircleShape::_NoModify_GetPointCount()
    {
        return point_count;
    }

    void CircleShape::_ResetModify()
    {
        is_modified = false;
    }

    bool CircleShape::_IsModified() const
    {
        return is_modified;
    }
}