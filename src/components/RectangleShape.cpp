#include <doge/components/RectangleShape.hpp>

namespace doge
{
    RectangleShape::RectangleShape(
            const Vec2f& size,
            const Vec2f& origin,
            const Color& color
        ) :
        size(size),
        origin(origin),
        color(color)
    {
    }

    Vec2f& RectangleShape::GetSize()
    {
        is_modified = true;
        return size;
    }

    const Vec2f& RectangleShape::GetSize() const
    {
        return size;
    }

    Vec2f& RectangleShape::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& RectangleShape::GetOrigin() const
    {
        return origin;
    }

    Color& RectangleShape::GetColor()
    {
        is_modified = true;
        return color;
    }

    const Color& RectangleShape::GetColor() const
    {
        return color;    
    }

    Vec2f& RectangleShape::_NoModify_GetSize()
    {
        return size;
    }

    Vec2f& RectangleShape::_NoModify_GetOrigin()
    {
        return origin;
    }

    Color& RectangleShape::_NoModify_GetColor()
    {
        return color;
    }

    void RectangleShape::_ResetModify()
    {
        is_modified = false;
    }

    bool RectangleShape::_IsModified() const
    {
        return is_modified;
    }
}