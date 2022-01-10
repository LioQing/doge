#pragma once

#include <string>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Window.hpp>

namespace doge
{
    struct WindowSettings
    {
        enum Mode
        {
            Windowed,
            FullScreen,
            Borderless,
        };

        enum Style
        {
            None = 0,
            Titlebar = 1 << 0,
            Resizable = 1 << 1,
            Close = 1 << 2,
            All = Titlebar | Resizable | Close,
        };

        Vec2u resolution = Vec2u(800, 600);
        std::string title = "";
        std::uint32_t fps = 60;
        Mode mode = Mode::Windowed;
        Style window_style = Style::All;
    };
}