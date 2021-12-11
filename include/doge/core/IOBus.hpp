#pragma once

#include <concepts>
#include <SFML/Graphics.hpp>

#include "VideoSettings.hpp"

namespace doge
{
    struct Engine;

    struct IOBus
    {
        sf::RenderWindow window;
        sf::Clock clock;

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

        void Render(const Engine& e);

        void Display();

        void SetFrameRate(uint32_t frame_rate);

        void StartDeltaClock();

        float GetDeltaTime();
    };
}