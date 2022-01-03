#include <doge/components/CompoundShape.hpp>

namespace doge
{
    CompoundShape::CompoundShape(
            const std::vector<ConvexShape>& convex_shapes,
            const std::vector<CircleShape>& circle_shapes,
            const std::vector<RectangleShape>& rectangle_shapes
        ) :
        convex_shapes(convex_shapes),
        circle_shapes(circle_shapes),
        rectangle_shapes(rectangle_shapes)
    {
    }

    std::vector<ConvexShape>& CompoundShape::GetConvexShapes()
    {
        is_modified = true;
        return convex_shapes;
    }

    const std::vector<ConvexShape>& CompoundShape::GetConvexShapes() const
    {
        return convex_shapes;
    }

    std::vector<CircleShape>& CompoundShape::GetCircleShapes()
    {
        is_modified = true;
        return circle_shapes;
    }

    const std::vector<CircleShape>& CompoundShape::GetCircleShapes() const
    {
        return circle_shapes;
    }

    std::vector<RectangleShape>& CompoundShape::GetRectangleShapes()
    {
        is_modified = true;
        return rectangle_shapes;
    }

    const std::vector<RectangleShape>& CompoundShape::GetRectangleShapes() const
    {
        return rectangle_shapes;
    }

    std::vector<ConvexShape>& CompoundShape::_NoModify_GetConvexShapes()
    {
        return convex_shapes;
    }

    std::vector<CircleShape>& CompoundShape::_NoModify_GetCircleShapes()
    {
        return circle_shapes;
    }

    std::vector<RectangleShape>& CompoundShape::_NoModify_GetRectangleShapes()
    {
        return rectangle_shapes;
    }

    void CompoundShape::_ResetModify()
    {
        is_modified = false;
    }

    bool CompoundShape::_IsModified() const
    {
        return is_modified;
    }
}