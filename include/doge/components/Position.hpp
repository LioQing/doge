#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    class Position
    {
        Vec2f position = Vec2f::Zero();

        bool is_modified = true;

    public:

        Position(const Position&) = default;
        Position(const Vec2f& position = Vec2f::Zero());
        Position(float x, float y);

        Vec2f& Get();
        const Vec2f& Get() const;

        Vec2f& _NoModify_Get();
        void _ResetModify();
        bool _IsModified() const;
    };
}