#pragma once

#include <concepts>
#include <unordered_map>
#include <memory>
#include <variant>
#include <array>
#include <set>
#include <SFML/Graphics.hpp>
#include <doge/core/WindowSettings.hpp>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;

    struct IOBus
    {
        using Texture = sf::Texture;
        using Image = sf::Image;

        sf::RenderWindow window;
        sf::Clock clock;
        std::unordered_map<EntityID, std::pair<std::unique_ptr<sf::View>, std::set<std::tuple<EntityID, std::uint8_t, std::size_t>>>> views_draws;
        std::unordered_map<EntityID, std::variant<std::unique_ptr<sf::Drawable>, std::array<std::vector<std::unique_ptr<sf::Drawable>>, 3>>> drawables;

        void CreateWindow(const WindowSettings& window_settings);

        void CloseWindow();

        template <typename Functor>
        requires std::invocable<Functor, sf::Event>
        void PollEvent(Functor functor)
        {
            sf::Event event;
            while (window.pollEvent(event))
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

        void SetFrameRate(uint32_t frame_rate);

        void StartDeltaClock();

        float GetDeltaTime();
    };
}