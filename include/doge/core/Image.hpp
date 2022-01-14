#pragma once

#include <doge/core/io/Window.hpp>

namespace doge
{
    struct Texture;

    struct Image
    {
        io::Image image_io;

        Image(const Image&) = default;
        Image(const io::Image& texture);

        void Create(const Vec2u& size, const Color& color = Color::Black());
        bool FromFile(const std::string& filename);
        bool FromMemory(void* data, std::size_t size);
        void FromTexture(const Texture& texture);

        bool ToFile(const std::string& filename) const;

        Vec2u GetSize() const;

        void MaskColor(const Color& color, std::uint8_t alpha = 0);
        void SetPixel(std::uint32_t x, std::uint32_t y, const Color& color);
        Color GetPixel(std::uint32_t x, std::uint32_t y) const;
        const std::uint8_t* GetPixelPtr() const;

        void FlipHorizontally();
        void FlipVertically();
    };
}