#pragma once

#include <doge/utils/lev.hpp>
#include <doge/core/io/Keyboard.hpp>

namespace doge::event
{
    struct Key
    {
        using Code = io::Keyboard::Key;

        Code key;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };
};