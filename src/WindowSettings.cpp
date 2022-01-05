#include <doge/core/WindowSettings.hpp>

namespace doge
{
    WindowSettings::WindowSettings(uint32_t width, uint32_t height, const std::string& title, uint32_t fps, Mode mode, WindowStyle window_style)
        : resolution(width, height), mode(mode), window_style(window_style), fps(fps), title(title)
    {
    }

    WindowSettings::WindowSettings(Vec2u resolution, const std::string& title, uint32_t fps, Mode mode, WindowStyle window_style)
        : resolution(resolution), mode(mode), window_style(window_style), fps(fps), title(title)
    {
    }
}