#pragma once

#include <doge/utils/lev.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::event
{
    struct Key
    {
        io::Input::Keyboard::Key key;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };
};