#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::event
{
    struct MouseWheel
    {
        using Wheel = io::Input::Mouse::Wheel;

        Wheel wheel;
        float delta;
        Vec2i position;
    };
};