#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::io
{
    struct Texture;
    
    struct Image
    {
        sf::Image image;

        Vec2u GetSize() const;

        void Create(const Vec2u& size, const Color& color = Color::Black());
        bool FromFile(const std::string& filename);
        bool FromMemory(void* data, std::size_t size);
        void DownloadFromTexture(const Texture& texture);

        bool ToFile(const std::string& filename) const;

        void MaskColor(const Color& color, std::uint8_t alpha = 0);
        void SetPixel(std::uint32_t x, std::uint32_t y, const Color& color);
        Color GetPixel(std::uint32_t x, std::uint32_t y) const;
        const std::uint8_t* GetPixelPtr() const;

        void FlipHorizontally();
        void FlipVertically();
    };
}