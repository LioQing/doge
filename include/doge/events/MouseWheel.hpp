#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Mouse.hpp>

namespace doge::event
{
    struct MouseWheel
    {
        using Wheel = io::Mouse::Wheel;

        Wheel wheel;
        float delta;
        Vec2i position;
    };
};