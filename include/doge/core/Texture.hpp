#pragma once

#include <doge/core/io/Texture.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <vector>
#include <variant>

namespace doge
{
    struct Image;
    
    struct Texture
    {
        struct SpriteSheet
        {
            enum Type
            {
                None, VariedSize, FixedSize,
            };

            Type GetType() const;
            void SetNone();
            void SetVariedSize(const std::vector<Recti>& frame);
            void SetFixedSize(const Vec2u& size);

            Recti GetFrame(std::size_t index) const;
            Recti GetFrame(const Vec2i& indices) const;

        private:

            Type type = Type::None;
            std::variant<std::nullptr_t, std::vector<Recti>, Vec2u> frame = nullptr;
        };

        enum RenderOptions
        {
            None = 0,
            Smooth = 1 << 0,
            sRGB = 1 << 1,
            Repeated = 1 << 2,
        };
        
        io::Texture texture_io = io::Texture();
        SpriteSheet sprite_sheet = SpriteSheet();

        Texture() = default;
        Texture(const Texture&) = default;
        Texture(const io::Texture& texture);

        bool Create(const Vec2u& size);
        bool FromFile(const std::string& filename, const Recti& area = Recti());
        bool FromMemory(void* data, std::size_t size, const Recti& area = Recti());
        bool FromImage(const Image& image, const Recti& area = Recti());

        void SetRenderOptions(RenderOptions options);
        bool IsSmooth() const;
        bool IsSRGB() const;
        bool IsRepeated() const;

        Vec2u GetSize() const;
    };
}