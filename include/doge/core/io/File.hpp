#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Color.hpp>
#include <string>
#include <SFML/Graphics.hpp>

namespace doge
{
    namespace io
    {
        struct File
        {
            struct Texture;
            struct Image;

            struct Texture
            {
                sf::Texture texture;

                Vec2u GetSize() const;

                bool Create(const Vec2u& size);
                bool FromFile(const std::string& filename, const Recti& area = Recti());
                bool FromMemory(void* data, std::size_t size, const Recti& area = Recti());
                bool FromImage(const Image& image, const Recti& area = Recti());

                void SetSmooth(bool smooth);
                bool IsSmooth() const;

                void SetSRGB(bool srgb);
                bool IsSRGB() const;

                void SetRepeated(bool repeated);
                bool IsRepeated() const;
            };

            struct Image
            {
                sf::Image image;

                Vec2u GetSize() const;

                void Create(const Vec2u& size, const Color& color = Color::Black());
                bool FromFile(const std::string& filename);
                bool FromMemory(void* data, std::size_t size);
                void FromTexture(const Texture& texture);

                bool ToFile(const std::string& filename) const;

                void MaskColor(const Color& color, std::uint8_t alpha = 0);
                void SetPixel(std::uint32_t x, std::uint32_t y, const Color& color);
                Color GetPixel(std::uint32_t x, std::uint32_t y) const;
                const std::uint8_t* GetPixelPtr() const;

                void FlipHorizontally();
                void FlipVertically();
            };
        };
    }
}