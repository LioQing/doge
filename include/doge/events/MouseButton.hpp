#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/InputDevice.hpp>

namespace doge::event
{
    struct MouseButton
    {
        io::InputDevice::Mouse::Button button;
        Vec2i position;
    };
};