#pragma once

#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    class CircleShape
    {
        float radius = 1.f;
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
        std::size_t point_count = 32;

        bool is_modified = true;

    public:

        CircleShape(const CircleShape&) = default;
        CircleShape(
            float radius = 1.f,
            const Vec2f& origin = Vec2f::Zero(),
            const Color& color = Color::White(),
            std::size_t point_count = 32
        );

        float& GetRadius();
        float GetRadius() const;

        Vec2f& GetOrigin();
        const Vec2f& GetOrigin() const;

        Color& GetColor();
        const Color& GetColor() const;

        std::size_t& GetPointCount();
        std::size_t GetPointCount() const;

        float& _NoModify_GetRadius();
        Vec2f& _NoModify_GetOrigin();
        Color& _NoModify_GetColor();
        std::size_t& _NoModify_GetPointCount();
        void _ResetModify();
        bool _IsModified() const;
    };
}