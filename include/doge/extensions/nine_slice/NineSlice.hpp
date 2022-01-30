#pragma once

#include <string>
#include <unordered_map>
#include <doge/utils/Rect.hpp>
#include <doge/core/Component.hpp>
#include <doge/extensions/nine_slice/NineSliceTexture.hpp>
#include <doge/core/TextureEx.hpp>

namespace doge
{
    struct CompoundSprite;
    struct Assets;

    namespace nine_slice
    {
        struct NineSlice
        {
            using Texture = NineSliceTexture;

            enum BorderThickness
            {
                Unchange,
                Stretch,
                TileScale,
                HorizontalScale,
                VerticalScale,
            };

            NineSlice(Engine& engine);

            static std::string TextureIDFromSlice(NineSliceTexture::Slice slice, const std::string& id);
            static std::string SliceTextureIDFromTextureID(const std::string& id, NineSliceTexture::Slice slice);
            static std::string SliceTextureIDFromCompoundSprite(const CompoundSprite& comp_sprite);

            std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
            AddTexture(const std::string& id, const Recti& border_thickness, const Recti& texture_rectangle);

            void EraseTexture(const std::string& id);

            const std::unordered_map<std::string, NineSliceTexture>& GetTextures() const;

            std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
            LoadTexture(const std::string& id, const std::string& filename, const Recti& border_thickness, const Recti& area = Recti());

            std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
            LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& border_thickness, const Recti& area = Recti());

            std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
            LoadTexture(const std::string& id, const io::Image& image, const Recti& border_thickness, const Recti& area = Recti());

            void SetRenderOptions(const std::string& id, TextureEx::RenderOptions options);

            void SetSmooth(const std::string& id, bool smooth);
            bool IsSmooth(const std::string& id) const;

            void SetSRGB(const std::string& id, bool srgb);
            bool IsSRGB(const std::string& id) const;

            void SetRepeated(const std::string& id, bool repeated);
            bool IsRepeated(const std::string& id) const;

            Component<CompoundSprite>& Add9SliceSpriteBySize(
                Entity entity,
                const std::string& texture_id,
                const Vec2f& size = Vec2f::Zero,
                const Vec2i& center_texture_size = Vec2i::Zero,
                const Rectf& border_thickness = Rectf(),
                const Vec2f& origin = Vec2f::Zero,
                const Color& color = Color::White
            ) const;
            
            Component<CompoundSprite>& Add9SliceSpriteByTile(
                Entity entity,
                const std::string& texture_id,
                const Vec2f& tile_size,
                const Vec2u& tile_count = Vec2u::One,
                BorderThickness border_thickness = BorderThickness::Stretch,
                const Vec2f& origin = Vec2f::Zero,
                const Color& color = Color::White
            ) const;

            void SetSpriteTextureID(CompoundSprite& comp_sprite, const std::string& id) const;
            void SetSpriteSizeAndBorder(CompoundSprite& comp_sprite, const Vec2f& size, const Rectf& border_thickness) const;
            void SetSpriteOrigin(CompoundSprite& comp_sprite, const Vec2f& origin) const;
            void SetSpriteColor(CompoundSprite& comp_sprite, const Color& color) const;
            void SetSpriteCenterTextureSize(CompoundSprite& comp_sprite, const Vec2f& center_texture_size) const;

        private:

            void Update9SliceSprite(
                CompoundSprite& comp_sprite,
                const std::string& texture_id,
                const Vec2f& center_texture_size,
                const Vec2f& size,
                const Rectf& border_thickness,
                const Vec2f& origin,
                const Color& color
            ) const;

            Engine& engine;

            std::unordered_map<std::string, NineSliceTexture> textures;
        };
    }
}