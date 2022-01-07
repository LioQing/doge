#pragma once

#include <doge/core/IOBus.hpp>
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

        struct RenderOptions
        {
            enum Options
            {
                None = 0,
                Smooth = 1 << 0,
                sRGB = 1 << 1,
                Repeated = 1 << 2,
            };

            RenderOptions(IOBus::TextureData& texture_data);

            void Set(Options options);
            bool IsSmooth() const;
            bool IsSRGB() const;
            bool IsRepeated() const;

        private:

            IOBus::TextureData& texture_data;
        };

        IOBus::TextureData texture_data = IOBus::TextureData();
        SpriteSheet sprite_sheet = SpriteSheet();
        RenderOptions render_options = RenderOptions(texture_data);

        Texture() = default;
        Texture(const Texture&) = default;
        Texture(const IOBus::TextureData& texture_data);
        Texture(const Vec2u& size);
        Texture(const std::string& filename, const Recti& area = Recti());
        Texture(void* data, std::size_t size, const Recti& area = Recti());
        Texture(const Image& image, const Recti& area = Recti());
    };
}