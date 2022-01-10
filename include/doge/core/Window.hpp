#pragma once

#include <string>
#include <doge/core/WindowSettings.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/IOBus.hpp>

namespace doge
{
    struct Window
    {
        using Settings = WindowSettings;

        Settings settings;
        IOBus io_bus;

        void CreateWindow();
        void CloseWindow();

        void SetFrameRate(uint32_t fps);
    };
}