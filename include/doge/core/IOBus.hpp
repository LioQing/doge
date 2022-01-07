#pragma once

#include <concepts>
#include <unordered_map>
#include <memory>
#include <variant>
#include <array>
#include <set>
#include <SFML/Graphics.hpp>
#include <doge/core/WindowSettings.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/aliases.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    struct Engine;

    struct IOBus
    {
        // window

        sf::RenderWindow window;
        sf::Clock clock;

        // game loops

        std::unordered_map<EntityID, std::pair<std::unique_ptr<sf::View>, std::set<std::tuple<EntityID, std::uint8_t, std::size_t>>>> views_draws;
        std::unordered_map<EntityID, std::variant<std::unique_ptr<sf::Drawable>, std::array<std::vector<std::unique_ptr<sf::Drawable>>, 3>>> drawables;

        // textures and images

        struct TextureData;
        struct ImageData;

        struct TextureData
        {
            sf::Texture texture;

            Vec2u GetSize() const;

            bool Create(const Vec2u& size);
            bool FromFile(const std::string& filename, const Recti& area = Recti());
            bool FromMemory(void* data, std::size_t size, const Recti& area = Recti());
            bool FromImage(const ImageData& image, const Recti& area = Recti());

            void SetSmooth(bool smooth);
            bool IsSmooth() const;

            void SetSRGB(bool srgb);
            bool IsSRGB() const;

            void SetRepeated(bool repeated);
            bool IsRepeated() const;
        };

        struct ImageData
        {
            sf::Image image;

            Vec2u GetSize() const;

            void Create(const Vec2u& size, const Color& color = Color::Black());
            bool FromFile(const std::string& filename);
            bool FromMemory(void* data, std::size_t size);
            void FromTexture(const TextureData& texture);

            bool ToFile(const std::string& filename) const;

            void MaskColor(const Color& color, std::uint8_t alpha = 0);
            void SetPixel(std::uint32_t x, std::uint32_t y, const Color& color);
            Color GetPixel(std::uint32_t x, std::uint32_t y) const;
            const std::uint8_t* GetPixelPtr() const;

            void FlipHorizontally();
            void FlipVertically();
        };

        // window

        void CreateWindow(const WindowSettings& window_settings);

        void CloseWindow();

        // game loops

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

        // textures and images

    };
}