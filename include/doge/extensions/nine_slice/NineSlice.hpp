#pragma once

#include <string>
#include <unordered_map>
#include <doge/utils/Rect.hpp>
#include <doge/core/Component.hpp>
#include <doge/extensions/nine_slice/Texture.hpp>
#include <doge/extensions/nine_slice/Slice.hpp>
#include <doge/extensions/nine_slice/Sprite.hpp>
#include <doge/core/io/Texture.hpp>

namespace doge
{
    struct Assets;

    namespace nine_slice
    {
        struct NineSlice
        {
            NineSlice(Engine& engine);
            ~NineSlice();

            static std::string SliceTextureIDFromTextureID(const std::string& id, Slice slice);
            static std::string TextureIDFromSliceTextureID(const std::string& id, Slice slice);

            std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
            AddTexture(const std::string& id, const Recti& border_thickness, const Recti& texture_rectangle);

            void EraseTexture(const std::string& id);

            const std::unordered_map<std::string, Texture>& GetTextures() const;
            const Texture& GetTexture(const std::string& id) const;
            const io::Texture& GetSliceTexture(const std::string& id, Slice slice) const;

            std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
            LoadTexture(const std::string& id, const std::string& filename, const Recti& border_thickness, const Recti& area = Recti());

            std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
            LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& border_thickness, const Recti& area = Recti());

            std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
            LoadTexture(const std::string& id, const io::Image& image, const Recti& border_thickness, const Recti& area = Recti());

            void SetRenderOptions(const std::string& id, doge::io::Texture::RenderOptions options);

            void SetSmooth(const std::string& id, bool smooth);
            bool IsSmooth(const std::string& id) const;

            void SetSRGB(const std::string& id, bool srgb);
            bool IsSRGB(const std::string& id) const;

            void SetRepeated(const std::string& id, bool repeated);
            bool IsRepeated(const std::string& id) const;

            void UpdateSprite(Sprite& sprite) const;
            void SetSpriteTextureID(Sprite& sprite, const std::string& id) const;
            void SetSpriteSizeAndBorder(Sprite& sprite, const Vec2f& size, const Rectf& border_thickness) const;
            void SetSpriteOrigin(Sprite& sprite, const Vec2f& origin) const;
            void SetSpriteColor(Sprite& sprite, const Color& color) const;
            void SetSpriteCenterTextureSize(Sprite& sprite, const Vec2i& center_texture_size) const;

            std::string GetSpriteTextureID(const Sprite& sprite) const;
            const std::string& GetSpriteSliceTextureID(const Sprite& sprite, Slice slice) const;
            const Texture& GetSpriteTexture(const Sprite& sprite) const;
            const io::Texture& GetSpriteSliceTexture(const Sprite& sprite, Slice slice) const;
            const doge::Sprite& GetSprite(const Sprite& sprite, Slice slice) const;
            Vec2f GetSpriteSize(const Sprite& sprite) const;
            Vec2f GetSpriteActualSize(const Sprite& sprite, const Assets& assets) const;
            Rectf GetSpriteBorderThickness(const Sprite& sprite) const;
            const Vec2f& GetSpriteOrigin(const Sprite& sprite) const;
            const Color& GetSpriteColor(const Sprite& sprite) const;
            Vec2i GetSpriteCenterTextureSize(const Sprite& sprite) const;

        private:

            Engine& engine;

            std::unordered_map<std::string, Texture> textures;

            void LateUpdate(Engine& engine, DeltaTime dt);
            void Render(Engine& engine);
        };
    }
}