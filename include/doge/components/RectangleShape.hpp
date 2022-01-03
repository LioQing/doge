#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    class RectangleShape
    {
        Vec2f size = Vec2f::One();
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();

        bool is_modified = true;

    public:

        RectangleShape(const RectangleShape&) = default;
        RectangleShape(
            const Vec2f& size = Vec2f::One(),
            const Vec2f& origin = Vec2f::Zero(),
            const Color& color = Color::White()
        );

        Vec2f& GetSize();
        const Vec2f& GetSize() const;

        Vec2f& GetOrigin();
        const Vec2f& GetOrigin() const;

        Color& GetColor();
        const Color& GetColor() const;

        Vec2f& _NoModify_GetSize();
        Vec2f& _NoModify_GetOrigin();
        Color& _NoModify_GetColor();
        void _ResetModify();
        bool _IsModified() const;
    };
}