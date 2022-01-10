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
#include <doge/core/io/File.hpp>

namespace doge
{
    struct Engine;
    struct Window;
    struct WindowSettings;

    namespace io
    {
        struct Window
        {
            std::shared_ptr<sf::RenderWindow> window_sptr = std::make_shared<sf::RenderWindow>();
            sf::Clock clock;
            int style = sf::Style::Default;

            enum DrawableType
            {
                Convex, Circle, Rectangle, SpriteType, Custom
            };

            using DrawableKey = std::tuple<EntityID, DrawableType, std::size_t>;
            
            std::unordered_map<EntityID, std::pair<std::unique_ptr<sf::View>, std::set<DrawableKey>>> views_draws;
            std::map<DrawableKey, std::unique_ptr<sf::Drawable>> drawables;

            void CreateWindow(const WindowSettings& settings);

            void CloseWindow();

            template <typename Functor>
            requires std::invocable<Functor, sf::Event>
            void PollEvent(Functor functor)
            {
                sf::Event event;
                while (window_sptr->pollEvent(event))
                {
                    if (event.type == sf::Event::Closed)
                    {
                        CloseWindow();
                    }

                    functor(event);
                }
            }

            void Render(const Engine& engine);

            void Display();

            // properties

            void ApplySettings(const WindowSettings& settings);

            void SetFrameRate(std::uint32_t frame_rate);

            void SetTitle(const std::string& title);

            void SetIcon(const File::Image& icon);

            void StartDeltaClock();

            float GetDeltaTimeRestart();

            bool IsOpen() const;

            Vec2i GetPosition() const;
            void SetPosition(const Vec2i& pos);

            Vec2u GetSize() const;
            void SetSize(const Vec2u& size);
        };
    }
}