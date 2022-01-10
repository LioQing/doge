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
        window_io.SetFrameRate(fps);
        settings.fps = fps;
    }

    void Window::SetTitle(const std::string& title)
    {
        window_io.SetTitle(title);
        settings.title = title;
    }
}