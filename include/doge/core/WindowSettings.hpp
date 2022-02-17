#pragma once

#include <string>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Window.hpp>

namespace doge
{
    struct WindowSettings
    {
        enum Style : std::int32_t
        {
            None = 0,
            Titlebar = 1 << 0,
            Resizable = 1 << 1,
            Close = 1 << 2,
            Fullscreen = 1 << 3,

            Default = Titlebar | Resizable | Close,
        };

        Vec2u size = Vec2u(800, 600);
        std::string title = "";
        std::uint32_t fps = 60;
        std::int32_t style = Style::Default;
        bool v_sync = false;
        std::uint32_t msaa_level = 0;
    };
}