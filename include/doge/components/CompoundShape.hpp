#pragma once

#include <vector>
#include <doge/components/ConvexShape.hpp>
#include <doge/components/CircleShape.hpp>
#include <doge/components/RectangleShape.hpp>

namespace doge
{
    class CompoundShape
    {
        std::vector<ConvexShape> convex_shapes;
        std::vector<CircleShape> circle_shapes;
        std::vector<RectangleShape> rectangle_shapes;

        bool is_modified = true;

    public:

        CompoundShape(const CompoundShape&) = default;
        CompoundShape(
            const std::vector<ConvexShape>& convex_shapes = std::vector<ConvexShape>(),
            const std::vector<CircleShape>& circle_shapes = std::vector<CircleShape>(),
            const std::vector<RectangleShape>& rectangle_shapes = std::vector<RectangleShape>()
        );

        std::vector<ConvexShape>& GetConvexShapes();
        const std::vector<ConvexShape>& GetConvexShapes() const;

        std::vector<CircleShape>& GetCircleShapes();
        const std::vector<CircleShape>& GetCircleShapes() const;

        std::vector<RectangleShape>& GetRectangleShapes();
        const std::vector<RectangleShape>& GetRectangleShapes() const;

        std::vector<ConvexShape>& _NoModify_GetConvexShapes();
        std::vector<CircleShape>& _NoModify_GetCircleShapes();
        std::vector<RectangleShape>& _NoModify_GetRectangleShapes();
        void _ResetModify();
        bool _IsModified() const;
    };
}