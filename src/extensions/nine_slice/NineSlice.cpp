#include <doge/extensions/nine_slice/NineSlice.hpp>

#include <doge/extensions/nine_slice/Texture.hpp>
#include <doge/core/Engine.hpp>
#include <doge/core/Assets.hpp>
#include <doge/core/io/Texture.hpp>
#include <doge/core/TextureEx.hpp>
#include <doge/utils/math.hpp>

namespace doge::nine_slice
{
    NineSlice::NineSlice(Engine& engine) : engine(engine) 
    {
    }

    std::string NineSlice::TextureIDFromSlice(Texture::Slice slice, const std::string& id)
    {
        return std::string("nine_slice_") + id + "_" + Texture::PostFixFromSlice(slice);
    }

    std::string NineSlice::SliceTextureIDFromTextureID(const std::string& id, Texture::Slice slice)
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

        for (auto i = 0; i < Texture::Slice::Count; ++i)
            texture.textures.at(i) = TextureIDFromSlice(static_cast<Texture::Slice>(i), id);

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
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.texture.SetRenderOptions(options);
        }
    }

    void NineSlice::SetSmooth(const std::string& id, bool smooth)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.texture.SetSmooth(smooth);
        }
    }

    bool NineSlice::IsSmooth(const std::string& id) const
    {
        return engine.assets.GetTexture(TextureIDFromSlice(Texture::Slice::Center, id)).texture.IsSmooth();
    }

    void NineSlice::SetSRGB(const std::string& id, bool srgb)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.texture.SetSRGB(srgb);
        }
    }

    bool NineSlice::IsSRGB(const std::string& id) const
    {
        return engine.assets.GetTexture(TextureIDFromSlice(Texture::Slice::Center, id)).texture.IsSRGB();
    }

    void NineSlice::SetRepeated(const std::string& id, bool repeated)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = engine.assets.GetTexture(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.texture.SetRepeated(repeated);
        }   
    }

    bool NineSlice::IsRepeated(const std::string& id) const
    {
        return engine.assets.GetTexture(TextureIDFromSlice(Texture::Slice::Center, id)).texture.IsRepeated();
    }

    Component<CompoundSprite>& NineSlice::Add9SliceSpriteBySize(
        Entity entity,
        const std::string& texture_id,
        const Vec2f& size,
        const Vec2i& center_texture_size,
        const Rectf& border_thickness,
        const Vec2f& origin,
        const Color& color) const
    {
        auto& comp_sprite = entity.AddComponent<CompoundSprite>();

        Update9SliceSprite(comp_sprite, texture_id, center_texture_size, size, border_thickness, origin, color);

        return comp_sprite;
    }

    Component<CompoundSprite>& NineSlice::Add9SliceSpriteByTile(
        Entity entity,
        const std::string& texture_id,
        const Vec2f& tile_size,
        const Vec2u& tile_count,
        BorderThickness border_thickness_option,
        const Vec2f& origin,
        const Color& color) const
    {
        auto& comp_sprite = entity.AddComponent<CompoundSprite>();
        auto& slice_tex = textures.at(texture_id);

        auto texture_center_size = engine.assets.GetTexture(TextureIDFromSlice(Texture::Slice::Center, texture_id)).texture.GetSize();
        auto center_texture_size = texture_center_size * tile_count;

        auto size = tile_size * tile_count;

        auto border_thickness = slice_tex.border_thickness;
        if (border_thickness_option == BorderThickness::Stretch)
        {
            border_thickness.SetPosition(border_thickness.GetPosition() * size / slice_tex.texture_rectangle.GetSize());
            border_thickness.SetSize(border_thickness.GetSize() * size / slice_tex.texture_rectangle.GetSize());
        }
        else if (border_thickness_option == BorderThickness::TileScale)
        {
            border_thickness.SetPosition(border_thickness.GetPosition() * tile_size / slice_tex.texture_rectangle.GetSize());
            border_thickness.SetSize(border_thickness.GetSize() * tile_size / slice_tex.texture_rectangle.GetSize());
        }
        else if (border_thickness_option == BorderThickness::HorizontalScale)
        {
            border_thickness.SetPosition(border_thickness.GetPosition() * size.x / slice_tex.texture_rectangle.GetSize().x);
            border_thickness.SetSize(border_thickness.GetSize() * size.x / slice_tex.texture_rectangle.GetSize().x);
        }
        else if (border_thickness_option == BorderThickness::VerticalScale)
        {
            border_thickness.SetPosition(border_thickness.GetPosition() * size.y / slice_tex.texture_rectangle.GetSize().y);
            border_thickness.SetSize(border_thickness.GetSize() * size.y / slice_tex.texture_rectangle.GetSize().y);
        }

        Update9SliceSprite(comp_sprite, texture_id, center_texture_size, size, border_thickness, origin, color);

        return comp_sprite;
    }
    
    void NineSlice::SetSpriteTextureID(CompoundSprite& comp_sprite, const std::string& id) const
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            comp_sprite.sprites.at(i).texture_id = textures.at(id).textures.at(i);
        }
    }

    void NineSlice::SetSpriteSizeAndBorder(CompoundSprite& comp_sprite, const Vec2f& size, const Rectf& border_thickness) const
    {
        auto& slice_tex = GetTextures().at(GetSpriteTextureID(comp_sprite));
        
        auto rect_size = math::AutoSize(size, slice_tex.texture_rectangle.GetSize());
        auto bord_thick = border_thickness == Rectf() ? slice_tex.border_thickness.Cast<float>() : border_thickness;

        comp_sprite.sprites.at(Texture::Slice::Center)  .size = rect_size - bord_thick.GetPosition() - bord_thick.GetSize();
        
        comp_sprite.sprites.at(Texture::Slice::Top)     .size = comp_sprite.sprites.at(Texture::Slice::Center).size * Vec2f::i + bord_thick.top     * Vec2f::j;
        comp_sprite.sprites.at(Texture::Slice::Left)    .size = comp_sprite.sprites.at(Texture::Slice::Center).size * Vec2f::j + bord_thick.left    * Vec2f::i;
        comp_sprite.sprites.at(Texture::Slice::Bottom)  .size = comp_sprite.sprites.at(Texture::Slice::Center).size * Vec2f::i + bord_thick.height  * Vec2f::j;
        comp_sprite.sprites.at(Texture::Slice::Right)   .size = comp_sprite.sprites.at(Texture::Slice::Center).size * Vec2f::j + bord_thick.width   * Vec2f::i;

        comp_sprite.sprites.at(Texture::Slice::TopLeft)     .size = Vec2f(bord_thick.left,    bord_thick.top);
        comp_sprite.sprites.at(Texture::Slice::BottomLeft)  .size = Vec2f(bord_thick.left,    bord_thick.height);
        comp_sprite.sprites.at(Texture::Slice::BottomRight) .size = Vec2f(bord_thick.width,   bord_thick.height);
        comp_sprite.sprites.at(Texture::Slice::TopRight)    .size = Vec2f(bord_thick.width,   bord_thick.top);

        SetSpriteOrigin(comp_sprite, comp_sprite.sprites.at(Texture::Slice::TopLeft).origin);
    }

    void NineSlice::SetSpriteOrigin(CompoundSprite& comp_sprite, const Vec2f& origin) const
    {
        auto tl0 = Vec2f::Zero;
        auto tl1 = comp_sprite.sprites.at(Texture::Slice::TopLeft).size;
        auto tl2 = tl1 + comp_sprite.sprites.at(Texture::Slice::Center).size;
        auto tl3 = tl2 + comp_sprite.sprites.at(Texture::Slice::BottomRight).size;
        
        comp_sprite.sprites.at(Texture::Slice::Center)      .origin = origin - tl1;
        comp_sprite.sprites.at(Texture::Slice::TopLeft)     .origin = origin - tl0;
        comp_sprite.sprites.at(Texture::Slice::BottomLeft)  .origin = origin - Vec2f(0, tl2.y);
        comp_sprite.sprites.at(Texture::Slice::BottomRight) .origin = origin - tl2;
        comp_sprite.sprites.at(Texture::Slice::TopRight)    .origin = origin - Vec2f(tl2.x, 0);
        comp_sprite.sprites.at(Texture::Slice::Top)         .origin = origin - Vec2f(tl1.x, 0);
        comp_sprite.sprites.at(Texture::Slice::Left)        .origin = origin - Vec2f(0, tl1.y);
        comp_sprite.sprites.at(Texture::Slice::Bottom)      .origin = origin - Vec2f(tl1.x, tl2.y);
        comp_sprite.sprites.at(Texture::Slice::Right)       .origin = origin - Vec2f(tl2.x, tl1.y);
    }

    void NineSlice::SetSpriteColor(CompoundSprite& comp_sprite, const Color& color) const
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            comp_sprite.sprites.at(i).color = color;
        }
    }

    void NineSlice::SetSpriteCenterTextureSize(CompoundSprite& comp_sprite, const Vec2i& center_texture_size) const
    {
        comp_sprite.sprites.at(Texture::Slice::Center)  .texture_rectangle.SetSize(center_texture_size);

        comp_sprite.sprites.at(Texture::Slice::Top)     .texture_rectangle.SetSize(center_texture_size * Vec2i::i);
        comp_sprite.sprites.at(Texture::Slice::Left)    .texture_rectangle.SetSize(center_texture_size * Vec2i::j);
        comp_sprite.sprites.at(Texture::Slice::Bottom)  .texture_rectangle.SetSize(center_texture_size * Vec2i::i);
        comp_sprite.sprites.at(Texture::Slice::Right)   .texture_rectangle.SetSize(center_texture_size * Vec2i::j);
    }

    std::string NineSlice::GetSpriteTextureID(const CompoundSprite& comp_sprite) const
    {
        return SliceTextureIDFromTextureID(comp_sprite.sprites.at(Texture::Slice::TopLeft).texture_id, Texture::Slice::TopLeft);
    }

    const std::string& NineSlice::GetSpriteTextureID(const CompoundSprite& comp_sprite, Texture::Slice slice) const
    {
        return comp_sprite.sprites.at(slice).texture_id;
    }

    const Sprite& NineSlice::GetSprite(const CompoundSprite& comp_sprite, Texture::Slice slice) const
    {
        return comp_sprite.sprites.at(slice);
    }

    Vec2f NineSlice::GetSpriteSize(const CompoundSprite& comp_sprite) const
    {
        return GetSprite(comp_sprite, Texture::Slice::TopLeft).size
        + GetSprite(comp_sprite, Texture::Slice::Center).size
        + GetSprite(comp_sprite, Texture::Slice::BottomRight).size;
    }

    Rectf NineSlice::GetSpriteBorderThickness(const CompoundSprite& comp_sprite) const
    {
        return Rectf(GetSprite(comp_sprite, Texture::Slice::TopLeft).size, GetSprite(comp_sprite, Texture::Slice::BottomRight).size);
    }

    const Vec2f& NineSlice::GetSpriteOrigin(const CompoundSprite& comp_sprite) const
    {
        return GetSprite(comp_sprite, Texture::Slice::TopLeft).origin;
    }

    const Color& NineSlice::GetSpriteColor(const CompoundSprite& comp_sprite) const
    {
        return GetSprite(comp_sprite, Texture::Slice::TopLeft).color;
    }

    Vec2i NineSlice::GetSpriteCenterTextureSize(const CompoundSprite& comp_sprite) const
    {
        return GetSprite(comp_sprite, Texture::Slice::Center).texture_rectangle.GetSize();
    }

    void NineSlice::Update9SliceSprite(
        CompoundSprite& comp_sprite,
        const std::string& texture_id,
        const Vec2f& center_texture_size,
        const Vec2f& size,
        const Rectf& border_thickness,
        const Vec2f& origin,
        const Color& color) const
    {
        auto tl0 = Vec2f::Zero;
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = size - border_thickness.GetSize();
        auto tl3 = size;

        comp_sprite.sprites.resize(Texture::Slice::Count);

        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            comp_sprite.sprites.at(i).texture_id = textures.at(texture_id).textures.at(i);
            comp_sprite.sprites.at(i).color = color;
        }

        SetSpriteSizeAndBorder(comp_sprite, size, border_thickness);
        SetSpriteOrigin(comp_sprite, origin);
        SetSpriteCenterTextureSize(comp_sprite, center_texture_size);
    }
}