#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    class Velocity
    {
        Vec2f velocity = Vec2f::Zero();

        bool is_modified = true;

    public:

        Velocity(const Velocity&) = default;
        Velocity(const Vec2f& velocity = Vec2f::Zero());
        Velocity(float x, float y);

        Vec2f& Get();
        const Vec2f& Get() const;

        Vec2f& _NoModify_Get();
        void _ResetModify();
        bool _IsModified() const;
    };
}