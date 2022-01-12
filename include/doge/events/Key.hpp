#pragma once

#include <doge/utils/lev.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::event
{
    struct Key
    {
        using Code = io::Input::Keyboard::Key;

        Code key;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };
};