#pragma once

#include <doge/core/IOBus.hpp>

namespace doge
{
    struct Texture;

    struct Image
    {
        IOBus::ImageData image_data;

        Image(const Image&) = default;
        Image(const IOBus::ImageData& texture_data);
        Image(const Vec2u& size, const Color& color = Color::Black());
        Image(const std::string& filename);
        Image(void* data, std::size_t size);
        Image(const Texture& texture);

        Vec2u GetSize() const;

        void ToFile(const std::string& filename) const;

        void MaskColor(const Color& color, std::uint8_t alpha = 0);
        void SetPixel(std::uint32_t x, std::uint32_t y, const Color& color);
        Color GetPixel(std::uint32_t x, std::uint32_t y) const;
        const std::uint8_t* GetPixelPtr() const;

        void FlipHorizontally();
        void FlipVertically();
    };
}