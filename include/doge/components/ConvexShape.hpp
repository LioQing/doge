#pragma once

#include <vector>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    class ConvexShape
    {
        std::vector<Vec2f> points;
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();

        bool is_modified = true;

    public:

        ConvexShape(const ConvexShape&) = default;
        ConvexShape(
            const std::vector<Vec2f>& points = std::vector<Vec2f>(),
            const Vec2f& origin = Vec2f::Zero(),
            const Color& color = Color::White()
        );

        std::vector<Vec2f>& GetPoints();
        const std::vector<Vec2f>& GetPoints() const;

        Vec2f& GetOrigin();
        const Vec2f& GetOrigin() const;

        Color& GetColor();
        const Color& GetColor() const;

        std::vector<Vec2f>& _NoModify_GetPoints();
        Vec2f& _NoModify_GetOrigin();
        Color& _NoModify_GetColor();
        void _ResetModify();
        bool _IsModified() const;
    };
}