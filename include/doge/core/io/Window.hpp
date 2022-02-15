#pragma once

#include <concepts>
#include <unordered_map>
#include <map>
#include <memory>
#include <set>
#include <variant>
#include <array>
#include <optional>
#include <SFML/Graphics.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/lev.hpp>
#include <doge/core/io/Image.hpp>
#include <doge/core/io/Cursor.hpp>

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
            doge::Color background_color = doge::Color::Black;
            Image icon;

            enum DrawableType
            {
                Convex, Circle, Rectangle, Sprite, Custom, Text
            };

            using DrawableKey = std::tuple<EntityID, DrawableType, std::size_t>; // eid, draw type, index
            using ViewInfo = std::tuple<std::unique_ptr<sf::View>, std::set<DrawableKey>>; // view, draw key
            
            std::unordered_map<EntityID, ViewInfo> views_draws;
            std::map<std::int32_t, std::set<DrawableKey>> layers_draws;
            std::map<DrawableKey, std::unique_ptr<sf::Drawable>> drawables;

            std::map<std::int32_t, std::unordered_map<std::string, std::function<void(Engine&)>>> custom_draw_functions;

            doge::Event<sf::Event> sf_event;

            // window

            void CreateWindow(const WindowSettings& settings);

            void CloseWindow();

            void PollEvent();

            void Render(Engine& engine);

            void Draw(Engine& engine);

            void Display();

            Image TakeScreenshot() const;

            template <std::invocable<Engine&> F>
            void AddDrawFunction(std::int32_t layer, const std::string& id, F function)
            {
                if (!custom_draw_functions.contains(layer))
                    custom_draw_functions.emplace(layer, std::unordered_map<std::string, std::function<void(Engine&)>>());

                custom_draw_functions.at(layer).emplace(id, function);
            }

            void RemoveDrawFunction(std::int32_t layer, const std::string& id);

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

            void SetMouseCursorVisible(bool visible);

            void SetMouseCursorGrabbed(bool grabbed);

            void SetMouseCursor(const Cursor& cursor);

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
            std::uint32_t msaa_level = 0;
        };
    }
}