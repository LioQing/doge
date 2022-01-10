#include <doge/core/Window.hpp>

namespace doge
{
    void Window::CreateWindow()
    {
        window_io.CreateWindow(settings);
    }

    void Window::CloseWindow()
    {
        window_io.CloseWindow();
    }

    void Window::SetFrameRate(std::uint32_t fps)
    {
        settings.fps = fps;
    }
}