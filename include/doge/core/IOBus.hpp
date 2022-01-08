#pragma once

#include <concepts>
#include <unordered_map>
#include <map>
#include <memory>
#include <set>
#include <variant>
#include <array>
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

        sf::RenderWindow window;
        sf::Clock clock;

        void CreateWindow(const WindowSettings& window_settings);

        void CloseWindow();

        // game loops

        enum class DrawableType : uint8_t
        {
            Convex, Circle, Rectangle, Sprite, Custom
        };

        using DrawableKey = std::tuple<EntityID, DrawableType, std::size_t>;
        
        std::unordered_map<EntityID, std::pair<std::unique_ptr<sf::View>, std::set<DrawableKey>>> views_draws;
        std::map<DrawableKey, std::unique_ptr<sf::Drawable>> drawables;

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