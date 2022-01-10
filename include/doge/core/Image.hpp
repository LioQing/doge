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

        void Create(const Vec2u& size, const Color& color = Color::Black());
        bool FromFile(const std::string& filename);
        bool FromMemory(void* data, std::size_t size);
        void FromTexture(const Texture& texture);

        void ToFile(const std::string& filename) const;

        void MaskColor(const Color& color, std::uint8_t alpha = 0);
        void SetPixel(std::uint32_t x, std::uint32_t y, const Color& color);
        Color GetPixel(std::uint32_t x, std::uint32_t y) const;
        const std::uint8_t* GetPixelPtr() const;

        void FlipHorizontally();
        void FlipVertically();

        Vec2u GetSize() const;
    };
}