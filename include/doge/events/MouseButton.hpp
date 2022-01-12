#pragma once

#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Input.hpp>

namespace doge::event
{
    struct MouseButton
    {
        using Button = io::Input::Mouse::Button;

        Button button;
        Vec2i position;
    };
};