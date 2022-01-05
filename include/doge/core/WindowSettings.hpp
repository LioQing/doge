#pragma once

#include <doge/utils/Vec2.hpp>

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

        enum WindowStyle
        {
            None = 0,
            Titlebar = 1 << 0,
            Resizable = 1 << 1,
            Close = 1 << 2,
            All = Titlebar | Resizable | Close,
        };

        WindowSettings() = default;
        WindowSettings(uint32_t width, uint32_t height, const std::string& title = "", uint32_t fps = 60, Mode mode = Mode::Windowed, WindowStyle window_style = WindowStyle::All);
        WindowSettings(Vec2u size, const std::string& title = "", uint32_t fps = 60, Mode mode = Mode::Windowed, WindowStyle window_style = WindowStyle::All);

        Vec2u resolution = Vec2u(800, 600);
        WindowStyle window_style = WindowStyle::All;
        Mode mode = Mode::Windowed;
        uint32_t fps = 60;
        std::string title = "";
    };
}