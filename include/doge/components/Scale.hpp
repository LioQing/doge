#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct Scale
    {
        Scale() = default;
        Scale(const Scale&) = default;
        Scale(const Vec2f& scale);
        Scale(float x, float y);

        Vec2f scale = Vec2f::One();
    };
}