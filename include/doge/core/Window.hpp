#pragma once

#include <string>
#include <doge/core/WindowSettings.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Window.hpp>

namespace doge
{
    struct Window
    {
        using Settings = WindowSettings;

        Settings settings;
        io::Window window_io;

        void CreateWindow();
        void CloseWindow();

        void SetFrameRate(std::uint32_t fps);

        void SetTitle(const std::string& title);
    };
}