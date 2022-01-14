#pragma once

#include <concepts>
#include <unordered_map>
#include <map>
#include <memory>
#include <set>
#include <variant>
#include <array>
#include <SFML/Graphics.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/lev.hpp>
#include <doge/core/io/Image.hpp>

namespace doge
{
    struct Engine;
    struct Window;
    struct WindowSettings;

    namespace io
    {
        struct Window
        {
            sf::RenderWindow window;
            sf::Clock clock;
            doge::Color background_color = doge::Color::Black();
            Image icon;

            enum DrawableType
            {
                Convex, Circle, Rectangle, SpriteType, Polygon
            };

            using DrawableKey = std::tuple<EntityID, DrawableType, std::size_t>;
            
            std::unordered_map<EntityID, std::pair<std::unique_ptr<sf::View>, std::set<DrawableKey>>> views_draws;
            std::map<DrawableKey, std::unique_ptr<sf::Drawable>> drawables;

            doge::Event<sf::Event> sf_event;

            void CreateWindow(const WindowSettings& settings);

            void CloseWindow();

            void PollEvent();

            void Render(const Engine& engine);

            void Display();

            Image TakeScreenshot() const;

            // delta time

            void StartDeltaClock();

            float GetDeltaTimeRestart();

            // properties

            void ApplySettings(const WindowSettings& settings);

            void SetFrameRate(std::uint32_t frame_rate);

            void SetTitle(const std::string& title);

            void SetIcon(const Image& icon);

            void SetVisible(bool visible);

            void SetVSyncEnabled(bool enabled);

            bool IsOpen() const;

            Vec2i GetPosition() const;
            void SetPosition(const Vec2i& pos);

            Vec2u GetSize() const;
            void SetSize(const Vec2u& size);

            void RequestFocus();
            bool HasFocus() const;

            Vec2i GetClientAreaPosition() const;

            Vec2i GetMousePosition() const;
            void SetMousePosition(const Vec2i& position) const;

        private:
            
            int style = sf::Style::Default;
        };
    }
}