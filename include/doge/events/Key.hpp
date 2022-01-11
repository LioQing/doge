#pragma once

#include <doge/utils/lev.hpp>
#include <doge/core/io/InputDevice.hpp>

namespace doge::event
{
    struct Key
    {
        io::InputDevice::Keyboard::Key key;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };
};