#pragma once

#include <concepts>
#include <unordered_map>
#include <memory>
#include <variant>
#include <array>
#include <SFML/Graphics.hpp>
#include <doge/core/VideoSettings.hpp>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;

    struct IOBus
    {
        sf::RenderWindow window;
        sf::Clock clock;
        std::unordered_map<EntityID, std::pair<std::unique_ptr<sf::View>, std::vector<EntityID>>> views_draws;
        std::unordered_map<EntityID, std::variant<std::unique_ptr<sf::Drawable>, std::array<std::vector<std::unique_ptr<sf::Drawable>>, 3>>> drawables;

        void CreateWindow(const VideoSettings& video_settings, const std::string& title);

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