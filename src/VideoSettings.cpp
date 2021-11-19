#include <doge/core/VideoSettings.hpp>

namespace doge
{
    VideoSettings::VideoSettings(uint32_t width, uint32_t height, Mode mode, WindowStyle window_style)
        : resolution(width, height), mode(mode), window_style(window_style)
    {
    }

    VideoSettings::VideoSettings(Vec2u resolution, Mode mode, WindowStyle window_style)
        : resolution(resolution), mode(mode), window_style(window_style)
    {
    }
}