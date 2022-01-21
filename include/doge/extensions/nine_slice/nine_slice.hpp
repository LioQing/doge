#pragma once

#include <string>
#include <unordered_map>
#include <doge/utils/Rect.hpp>
#include <doge/core/Component.hpp>
#include <doge/extensions/nine_slice/NineSliceTexture.hpp>
#include <doge/core/TextureEx.hpp>
#include <doge/extensions/nine_slice/NineSliceSpriteFactory.hpp>

namespace doge
{
    struct CompoundSprite;
    struct Assets;

    struct nine_slice
    {
        using Texture = NineSliceTexture;
        using SpriteFactory = NineSliceSpriteFactory;

        nine_slice(const nine_slice&) = delete;

        static std::string TextureIDFromSlice(NineSliceTexture::Slice slice, const std::string& id);

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

        static Component<CompoundSprite>& Add9SliceSprite(Assets& assets, Entity entity, const SpriteFactory& factory);

        static Component<CompoundSprite>& Add9SliceSprite(
            Assets& assets,
            Entity entity,
            const std::string& texture_id,
            const Vec2f& size,
            const Vec2i& center_texture_size = Vec2i::Zero(),
            const Rectf& border_thickness = Rectf(),
            const Vec2f& origin = Vec2f::Zero(),
            const Color& color = Color::White());

    private:

        static std::unordered_map<std::string, NineSliceTexture> textures;
    };
}