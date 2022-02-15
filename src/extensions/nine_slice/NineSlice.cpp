#include <doge/extensions/nine_slice/NineSlice.hpp>

#include <doge/extensions/nine_slice/Texture.hpp>
#include <doge/core/Engine.hpp>
#include <doge/core/io/Renderer.hpp>
#include <doge/core/Assets.hpp>
#include <doge/core/io/Texture.hpp>
#include <doge/utils/math.hpp>

namespace doge::nine_slice
{
    NineSlice::NineSlice(Engine& engine) : engine(engine) 
    {
        GameLoopFunctions glf;
        glf.late_update = [&](Engine& engine, DeltaTime dt){ LateUpdate(engine, dt); };
        glf.render = [&](Engine& engine){ Render(engine); };
        
        engine.scenes.extensions.emplace("doge_nine_slice", glf);
    }

    NineSlice::~NineSlice()
    {
        for (auto& [id, texture] : GetTextures())
        {
            for (auto& s_id : texture.textures)
            {
                engine.assets.EraseTexture(s_id);
            }
        }

        engine.scenes.extensions.erase("doge_nine_slice");
    }

    std::string NineSlice::SliceTextureIDFromTextureID(const std::string& id, Slice slice)
    {
        return "nine_slice_" + id + "_" + Texture::PostFixFromSlice(slice);
    }

    std::string NineSlice::TextureIDFromSliceTextureID(const std::string& id, Slice slice)
    {
        auto prefix_size = std::string("nine_slice_").size();
        return id.substr(prefix_size, id.find(Texture::PostFixFromSlice(slice)) - prefix_size - 1);
    }

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    NineSlice::AddTexture(const std::string& id, const Recti& border_thickness, const Recti& texture_rectangle)
    {
        auto result = textures.emplace(id, Texture{ .texture_rectangle = texture_rectangle, .border_thickness = border_thickness});
        if (!result.second)
            return result;

        auto& texture = result.first->second;

        for (auto i = 0; i < Slice::Count; ++i)
            texture.textures.at(i) = SliceTextureIDFromTextureID(id, static_cast<Slice>(i));

        return result;
    }

    void NineSlice::EraseTexture(const std::string& id)
    {
        for (auto& texture_id : textures.at(id).textures)
            engine.assets.EraseTexture(texture_id);

        textures.erase(id);
    }

    const std::unordered_map<std::string, Texture>& NineSlice::GetTextures() const
    {
        return textures;
    }

    const Texture& NineSlice::GetTexture(const std::string& id) const
    {
        return textures.at(id);
    }

    const io::Texture& NineSlice::GetSliceTexture(const std::string& id, Slice slice) const
    {
        return engine.assets.GetTexture(SliceTextureIDFromTextureID(id, slice));
    }

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    NineSlice::LoadTexture(const std::string& id, const std::string& filename, const Recti& border_thickness, const Recti& area)
    {
        auto result = AddTexture(id, border_thickness, area);
        if (!result.second)
            return result;

        Recti rect;
        {
            io::Texture texture;
            auto path = Assets::SearchForTexture(filename);

            if (path == "")
            {
                std::cerr << "Cannot find path: " << filename << std::endl;
                return std::make_pair(textures.end(), false);
            }

            texture.FromFile(path, area);
            rect = math::AutoSize(area, texture.GetSize());
            result.first->second.texture_rectangle = rect;
        }

        auto tl0 = Vec2i::Zero;
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = rect.GetSize() - border_thickness.GetSize();
        auto tl3 = rect.GetSize();

        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_center",         filename, Recti(tl1, tl2 - tl1));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top_left",       filename, Recti(tl0, tl1));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top",            filename, Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top_right",      filename, Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_right",          filename, Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_right",   filename, Recti(tl2, tl3 - tl2));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom",         filename, Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_left",    filename, Recti(0, tl2.y, tl1.x, tl3.y - tl2.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_left",           filename, Recti(0, tl1.y, tl1.x, tl2.y - tl1.y));

        return result;
    }

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    NineSlice::LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& border_thickness, const Recti& area)
    {
        auto result = AddTexture(id, border_thickness, area);
        if (!result.second)
            return result;

        Recti rect;
        {
            io::Texture texture;
            texture.FromMemory(data, size, area);
            rect = math::AutoSize(area, texture.GetSize());
            result.first->second.texture_rectangle = rect;
        }

        auto tl0 = Vec2i::Zero;
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = rect.GetSize() - border_thickness.GetSize();
        auto tl3 = rect.GetSize();

        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_center",         data, size, Recti(tl1, tl2 - tl1));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top_left",       data, size, Recti(tl0, tl1));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top",            data, size, Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top_right",      data, size, Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_right",          data, size, Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_right",   data, size, Recti(tl2, tl3 - tl2));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom",         data, size, Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_left",    data, size, Recti(0, tl2.y, tl1.x, tl3.y - tl2.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_left",           data, size, Recti(0, tl1.y, tl1.x, tl2.y - tl1.y));

        return result;
    }

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    NineSlice::LoadTexture(const std::string& id, const io::Image& image, const Recti& border_thickness, const Recti& area)
    {
        auto result = AddTexture(id, border_thickness, area);
        if (!result.second)
            return result;

        Recti rect;
        {
            io::Texture texture;
            texture.FromImage(image, area);
            rect = math::AutoSize(area, texture.GetSize());
            result.first->second.texture_rectangle = rect;
        }

        auto tl0 = Vec2i::Zero;
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = rect.GetSize() - border_thickness.GetSize();
        auto tl3 = rect.GetSize();

        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_center",         image, Recti(tl1, tl2 - tl1));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top_left",       image, Recti(tl0, tl1));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top",            image, Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_top_right",      image, Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_right",          image, Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_right",   image, Recti(tl2, tl3 - tl2));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom",         image, Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_left",    image, Recti(0, tl2.y, tl1.x, tl3.y - tl2.y));
        engine.assets.LoadTexture(std::string("nine_slice_") + id + "_left",           image, Recti(0, tl1.y, tl1.x, tl2.y - tl1.y));

        return result;
    }

    void NineSlice::SetRenderOptions(const std::string& id, doge::io::Texture::RenderOptions options)
    {
        for (auto i = 0; i < Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(SliceTextureIDFromTextureID(id, static_cast<Slice>(i)));

            texture.SetRenderOptions(options);
        }
    }

    void NineSlice::SetSmooth(const std::string& id, bool smooth)
    {
        for (auto i = 0; i < Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(SliceTextureIDFromTextureID(id, static_cast<Slice>(i)));

            texture.SetSmooth(smooth);
        }
    }

    bool NineSlice::IsSmooth(const std::string& id) const
    {
        return engine.assets.GetTexture(SliceTextureIDFromTextureID(id, Slice::Center)).IsSmooth();
    }

    void NineSlice::SetSRGB(const std::string& id, bool srgb)
    {
        for (auto i = 0; i < Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(SliceTextureIDFromTextureID(id, static_cast<Slice>(i)));

            texture.SetSRGB(srgb);
        }
    }

    bool NineSlice::IsSRGB(const std::string& id) const
    {
        return engine.assets.GetTexture(SliceTextureIDFromTextureID(id, Slice::Center)).IsSRGB();
    }

    void NineSlice::SetRepeated(const std::string& id, bool repeated)
    {
        for (auto i = 0; i < Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(SliceTextureIDFromTextureID(id, static_cast<Slice>(i)));

            texture.SetRepeated(repeated);
        }   
    }

    bool NineSlice::IsRepeated(const std::string& id) const
    {
        return engine.assets.GetTexture(SliceTextureIDFromTextureID(id, Slice::Center)).IsRepeated();
    }

    void NineSlice::UpdateSprite(Sprite& sprite) const
    {
        for (auto i = 0; i < Slice::Count; ++i)
        {
            if (sprite.texture_id == "missing_texture")
                sprite.sprites.at(i).texture_id = "missing_texture";
            else
                sprite.sprites.at(i).texture_id = textures.at(sprite.texture_id).textures.at(i);
            sprite.sprites.at(i).color = sprite.color;
        }

        SetSpriteSizeAndBorder(sprite, sprite.size, sprite.border_thickness);
        SetSpriteOrigin(sprite, sprite.origin);
        SetSpriteCenterTextureSize(sprite, sprite.center_texture_size);
    }

    void NineSlice::SetSpriteTextureID(Sprite& sprite, const std::string& id) const
    {
        sprite.texture_id = id;

        for (auto i = 0; i < Slice::Count; ++i)
        {
            sprite.sprites.at(i).texture_id = textures.at(id).textures.at(i);
        }
    }

    void NineSlice::SetSpriteSizeAndBorder(Sprite& sprite, const Vec2f& size, const Rectf& border_thickness) const
    {
        sprite.size = size;
        sprite.border_thickness = border_thickness;

        auto rect_size = math::AutoSize(size, doge::Vec2i(32, 32));
        auto bord_thick = Rectf();
        if (GetSpriteTextureID(sprite) != "missing_texture")
        {
            auto& slice_tex = GetTexture(GetSpriteTextureID(sprite));
            
            rect_size = math::AutoSize(size, slice_tex.texture_rectangle.GetSize());
            bord_thick = border_thickness == Rectf() ? slice_tex.border_thickness.Cast<float>() : border_thickness;
        }

        sprite.sprites.at(Slice::Center)  .size = rect_size - bord_thick.GetPosition() - bord_thick.GetSize();
        
        sprite.sprites.at(Slice::Top)     .size = sprite.sprites.at(Slice::Center).size * Vec2f::i + bord_thick.top     * Vec2f::j;
        sprite.sprites.at(Slice::Left)    .size = sprite.sprites.at(Slice::Center).size * Vec2f::j + bord_thick.left    * Vec2f::i;
        sprite.sprites.at(Slice::Bottom)  .size = sprite.sprites.at(Slice::Center).size * Vec2f::i + bord_thick.height  * Vec2f::j;
        sprite.sprites.at(Slice::Right)   .size = sprite.sprites.at(Slice::Center).size * Vec2f::j + bord_thick.width   * Vec2f::i;

        sprite.sprites.at(Slice::TopLeft)     .size = Vec2f(bord_thick.left,    bord_thick.top);
        sprite.sprites.at(Slice::BottomLeft)  .size = Vec2f(bord_thick.left,    bord_thick.height);
        sprite.sprites.at(Slice::BottomRight) .size = Vec2f(bord_thick.width,   bord_thick.height);
        sprite.sprites.at(Slice::TopRight)    .size = Vec2f(bord_thick.width,   bord_thick.top);

        SetSpriteOrigin(sprite, sprite.origin);
    }

    void NineSlice::SetSpriteOrigin(Sprite& sprite, const Vec2f& origin) const
    {
        sprite.origin = origin;

        auto tl0 = Vec2f::Zero;
        auto tl1 = sprite.sprites.at(Slice::TopLeft).size;
        auto tl2 = tl1 + sprite.sprites.at(Slice::Center).size;
        auto tl3 = tl2 + sprite.sprites.at(Slice::BottomRight).size;
        
        sprite.sprites.at(Slice::Center)      .origin = origin - tl1;
        sprite.sprites.at(Slice::TopLeft)     .origin = origin - tl0;
        sprite.sprites.at(Slice::BottomLeft)  .origin = origin - Vec2f(0, tl2.y);
        sprite.sprites.at(Slice::BottomRight) .origin = origin - tl2;
        sprite.sprites.at(Slice::TopRight)    .origin = origin - Vec2f(tl2.x, 0);
        sprite.sprites.at(Slice::Top)         .origin = origin - Vec2f(tl1.x, 0);
        sprite.sprites.at(Slice::Left)        .origin = origin - Vec2f(0, tl1.y);
        sprite.sprites.at(Slice::Bottom)      .origin = origin - Vec2f(tl1.x, tl2.y);
        sprite.sprites.at(Slice::Right)       .origin = origin - Vec2f(tl2.x, tl1.y);
    }

    void NineSlice::SetSpriteColor(Sprite& sprite, const Color& color) const
    {
        sprite.color = color;

        for (auto i = 0; i < Slice::Count; ++i)
        {
            sprite.sprites.at(i).color = color;
        }
    }

    void NineSlice::SetSpriteCenterTextureSize(Sprite& sprite, const Vec2i& center_texture_size) const
    {
        sprite.center_texture_size = center_texture_size;

        sprite.sprites.at(Slice::Center)  .texture_rectangle.SetSize(center_texture_size);

        sprite.sprites.at(Slice::Top)     .texture_rectangle.SetSize(center_texture_size * Vec2i::i);
        sprite.sprites.at(Slice::Left)    .texture_rectangle.SetSize(center_texture_size * Vec2i::j);
        sprite.sprites.at(Slice::Bottom)  .texture_rectangle.SetSize(center_texture_size * Vec2i::i);
        sprite.sprites.at(Slice::Right)   .texture_rectangle.SetSize(center_texture_size * Vec2i::j);
    }

    std::string NineSlice::GetSpriteTextureID(const Sprite& sprite) const
    {
        if (GetSpriteSliceTextureID(sprite, Slice::TopLeft) == "missing_texture")
            return "missing_texture";
        return TextureIDFromSliceTextureID(GetSpriteSliceTextureID(sprite, Slice::TopLeft), Slice::TopLeft);
    }

    const std::string& NineSlice::GetSpriteSliceTextureID(const Sprite& sprite, Slice slice) const
    {
        return sprite.sprites.at(slice).texture_id;
    }

    const Texture& NineSlice::GetSpriteTexture(const Sprite& sprite) const
    {
        return GetTexture(sprite.texture_id);
    }

    const io::Texture& NineSlice::GetSpriteSliceTexture(const Sprite& sprite, Slice slice) const
    {
        return GetSliceTexture(sprite.texture_id, slice);
    }

    const doge::Sprite& NineSlice::GetSprite(const Sprite& sprite, Slice slice) const
    {
        return sprite.sprites.at(slice);
    }

    Vec2f NineSlice::GetSpriteSize(const Sprite& sprite) const
    {
        return GetSprite(sprite, Slice::TopLeft).size
        + GetSprite(sprite, Slice::Center).size
        + GetSprite(sprite, Slice::BottomRight).size;
    }

    Vec2f NineSlice::GetSpriteActualSize(const Sprite& sprite, const Assets& assets) const
    {
        return GetSprite(sprite, Slice::TopLeft).GetActualSize(assets)
        + GetSprite(sprite, Slice::Center).GetActualSize(assets)
        + GetSprite(sprite, Slice::BottomRight).GetActualSize(assets);
    }

    Rectf NineSlice::GetSpriteBorderThickness(const Sprite& sprite) const
    {
        return Rectf(GetSprite(sprite, Slice::TopLeft).size, GetSprite(sprite, Slice::BottomRight).size);
    }

    const Vec2f& NineSlice::GetSpriteOrigin(const Sprite& sprite) const
    {
        return GetSprite(sprite, Slice::TopLeft).origin;
    }

    const Color& NineSlice::GetSpriteColor(const Sprite& sprite) const
    {
        return GetSprite(sprite, Slice::TopLeft).color;
    }

    Vec2i NineSlice::GetSpriteCenterTextureSize(const Sprite& sprite) const
    {
        return GetSprite(sprite, Slice::Center).texture_rectangle.GetSize();
    }

    void NineSlice::LateUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto [sprite] : engine.Select<Sprite>().Components())
        {
            UpdateSprite(sprite);
        }
    }

    void NineSlice::Render(Engine& engine)
    {
        for (auto [sprite_comp] : engine.Select<Sprite>().Components())
        {
            for (auto i = 0; i < Slice::Count; ++i)
            {
                io::Renderer::RenderSprite(engine, sprite_comp, sprite_comp.sprites.at(i), i);
            }
        }
    }
}