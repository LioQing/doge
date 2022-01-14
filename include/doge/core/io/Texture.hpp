#pragma once

#include <SFML/Graphics.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct Color;

    namespace io
    {
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
    }
}