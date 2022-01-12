#pragma once

#include <string>
#include <doge/core/WindowSettings.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Window.hpp>
#include <doge/core/Component.hpp>

namespace doge
{
    struct Camera;

    struct Window
    {
        using Settings = WindowSettings;

        Settings settings;
        io::Window window_io;

        void CreateWindow();
        void CloseWindow();

        void SetFrameRate(std::uint32_t fps);

        void SetTitle(const std::string& title);

        void SetIcon(const io::File::Image& icon);
        void SetIcon(const std::string& filename);

        void SetBackgroundColor(const Color& color);

        void SetVSyncEnabled(bool enabled);

        void SetSize(const Vec2u& size);

        Vec2i GetClientAreaPosition() const;

        bool IsOpen() const;

        io::File::Image TakeScreenshot() const;

        Vec2f MapPixelToCoords(const Vec2i& pixel, const Component<Camera>& camera) const;
        Vec2i MapCoordsToPixel(const Vec2f& coords, const Component<Camera>& camera) const;
    };
}