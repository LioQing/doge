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

    struct nine_slice
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

        nine_slice(const nine_slice&) = delete;

        static std::string TextureIDFromSlice(NineSliceTexture::Slice slice, const std::string& id);
        static std::string SliceTextureIDFromTextureID(const std::string& id, NineSliceTexture::Slice slice);

        std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
        static AddTexture(const std::string& id, const Recti& border_thickness, const Recti& texture_rectangle);

        static std::size_t EraseTexture(Assets& assets, const std::string& id);

        static const std::unordered_map<std::string, NineSliceTexture>& GetTextures();

        std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
        static LoadTexture(Assets& assets, const std::string& id, const std::string& filename, const Recti& border_thickness, const Recti& area = Recti());

        std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
        static LoadTexture(Assets& assets, const std::string& id, void* data, std::size_t size, const Recti& border_thickness, const Recti& area = Recti());

        std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
        static LoadTexture(Assets& assets, const std::string& id, const io::Image& image, const Recti& border_thickness, const Recti& area = Recti());

        static void SetRenderOptions(Assets& assets, const std::string& id, TextureEx::RenderOptions options);

        static void SetSmooth(Assets& assets, const std::string& id, bool smooth);
        static bool IsSmooth(const Assets& assets, const std::string& id);

        static void SetSRGB(Assets& assets, const std::string& id, bool srgb);
        static bool IsSRGB(const Assets& assets, const std::string& id);

        static void SetRepeated(Assets& assets, const std::string& id, bool repeated);
        static bool IsRepeated(const Assets& assets, const std::string& id);

        static Component<CompoundSprite>& Add9SliceSpriteBySize(
            Assets& assets,
            Entity entity,
            const std::string& texture_id,
            const Vec2f& size = Vec2f::Zero(),
            const Vec2i& center_texture_size = Vec2i::Zero(),
            const Rectf& border_thickness = Rectf(),
            const Vec2f& origin = Vec2f::Zero(),
            const Color& color = Color::White()
        );
        
        static Component<CompoundSprite>& Add9SliceSpriteByTile(
            Assets& assets,
            Entity entity,
            const std::string& texture_id,
            const Vec2f& tile_size,
            const Vec2u& tile_count = Vec2u::One(),
            BorderThickness border_thickness = BorderThickness::Stretch,
            const Vec2f& origin = Vec2f::Zero(),
            const Color& color = Color::White()
        );

        static void SetSpriteTextureID(CompoundSprite& comp_sprite, const std::string& id);
        static void SetSpriteSizeAndBorder(CompoundSprite& comp_sprite, const Vec2f& size, const Rectf& border_thickness);
        static void SetSpriteOrigin(CompoundSprite& comp_sprite, const Vec2f& origin);
        static void SetSpriteColor(CompoundSprite& comp_sprite, const Color& color);
        static void SetSpriteCenterTextureSize(CompoundSprite& comp_sprite, const Vec2f& center_texture_size);

    private:

        static void Update9SliceSprite(
            CompoundSprite& comp_sprite,
            const std::string& texture_id,
            const Vec2f& center_texture_size,
            const Vec2f& size,
            const Rectf& border_thickness,
            const Vec2f& origin,
            const Color& color
        );

        static std::unordered_map<std::string, NineSliceTexture> textures;
    };
}