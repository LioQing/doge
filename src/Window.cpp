#include <doge/core/Window.hpp>

namespace doge
{
    void Window::CreateWindow()
    {
        io_bus.CreateWindow(settings);
    }

    void Window::CloseWindow()
    {
        io_bus.CloseWindow();
    }

    void Window::SetFrameRate(std::uint32_t fps)
    {
        settings.fps = fps;
    }
}