#include <doge/core/Window.hpp>

#include <doge/core/Component.hpp>
#include <doge/components.hpp>

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

    void Window::SetIcon(const io::Image& icon)
    {
        window_io.icon = icon;
    }

    void Window::SetIcon(const std::string& filename)
    {
        window_io.icon.FromFile(filename);
    }

    void Window::SetBackgroundColor(const Color& color)
    {
        window_io.background_color = color;
    }

    void Window::SetVSyncEnabled(bool enabled)
    {
        window_io.SetVSyncEnabled(enabled);
        settings.v_sync = enabled;
    }

    void Window::SetSize(const Vec2u& position)
    {
        window_io.SetSize(position);
        settings.size = position;
    }

    Vec2i Window::GetClientAreaPosition() const
    {
        return window_io.GetClientAreaPosition();
    }

    bool Window::IsOpen() const
    {
        return window_io.IsOpen();
    }

    io::Image Window::TakeScreenshot() const
    {
        return window_io.TakeScreenshot();
    }

    Vec2f Window::GetAutoResizedCameraSize(const Camera& cam) const
    {
        return settings.size * cam.port.GetSize();
    }

    Vec2f Window::MapPixelToCoords(const Vec2i& pixel, const Component<Camera>& camera) const
    {
        auto entity = camera.GetEntity();
        auto size = window_io.GetSize();
        auto port_pos = camera.port.GetPosition() * size;
        auto port_size = camera.port.GetSize() * size;
        auto cam_size = camera.size;
        if (cam_size == Vec2f::Zero)
            cam_size = size;

        return ((pixel - port_pos - port_size / 2.f) * cam_size / port_size
            * global::GetScale(entity)).Rotated(global::GetRotation(entity)) + global::GetPosition(entity);
    }

    Vec2i Window::MapCoordsToPixel(const Vec2f& coords, const Component<Camera>& camera) const // global_pos = (local_pos - origin) * scale + pos
    {
        auto entity = camera.GetEntity();
        auto size = window_io.GetSize();
        auto port_pos = camera.port.GetPosition() * size;
        auto port_size = camera.port.GetSize() * size;
        auto cam_size = camera.size;
        if (cam_size == Vec2f::Zero)
            cam_size = size;

        return (coords - global::GetPosition(entity)).Rotated(-global::GetRotation(entity))
            / global::GetScale(entity) * port_size / cam_size + port_pos + port_size / 2.f;
    }
}