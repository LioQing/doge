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
    };
}