#pragma once

#include "../utils/Vec2.hpp"

namespace doge
{
    struct VideoSettings
    {
        enum Mode
        {
            Windowed,
            FullScreen,
            Borderless,
        };

        enum WindowStyle
        {
            None = 0,
            Titlebar = 1 << 0,
            Resizable = 1 << 1,
            Close = 1 << 2,
            All = Titlebar | Resizable | Close,
        };

        VideoSettings() = default;
        VideoSettings(uint32_t width, uint32_t height, Mode mode = Mode::Windowed, WindowStyle window_style = WindowStyle::All);
        VideoSettings(Vec2u size, Mode mode = Mode::Windowed, WindowStyle window_style = WindowStyle::All);

        Vec2u resolution = Vec2u(800, 600);
        WindowStyle window_style = WindowStyle::All;
        Mode mode = Mode::Windowed;
    };
}