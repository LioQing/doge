#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    class Scale
    {
        Vec2f scale = Vec2f::One();

        bool is_modified = true;

    public:

        Scale(const Scale&) = default;
        Scale(const Vec2f& scale = Vec2f::One());
        Scale(float x, float y);

        Vec2f& Get();
        const Vec2f& Get() const;

        Vec2f& _NoModify_Get();
        void _ResetModify();
        bool _IsModified() const;
    };
}