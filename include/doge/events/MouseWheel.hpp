#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/InputDevice.hpp>

namespace doge::event
{
    struct MouseWheel
    {
        io::InputDevice::Mouse::Wheel wheel;
        float delta;
        Vec2i position;
    };
};