#include <doge/extensions/nine_slice/nine_slice.hpp>

#include <doge/extensions/nine_slice/NineSliceTexture.hpp>
#include <doge/core/Assets.hpp>
#include <doge/core/io/Texture.hpp>
#include <doge/core/TextureEx.hpp>
#include <doge/utils/math.hpp>

namespace doge
{
    std::unordered_map<std::string, NineSliceTexture> nine_slice::textures;

    std::string nine_slice::TextureIDFromSlice(NineSliceTexture::Slice slice, const std::string& id)
    {
        return std::string("nine_slice_") + id + "_" + Texture::PostFixFromSlice(slice);
    }

    std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
    nine_slice::AddTexture(const std::string& id, const Recti& border_thickness, const Recti& texture_rectangle)
    {
        auto result = textures.emplace(id, Texture{ .texture_rectangle = texture_rectangle, .border_thickness = border_thickness});
        if (!result.second)
            return result;

        auto& texture = result.first->second;

        for (auto i = 0; i < Texture::Slice::Count; ++i)
            texture.textures.at(i) = TextureIDFromSlice(static_cast<Texture::Slice>(i), id);

        return result;
    }

    std::size_t nine_slice::EraseTexture(Assets& assets, const std::string& id)
    {
        for (auto& texture_id : textures.at(id).textures)
            assets.textures.erase(texture_id);

        return textures.erase(id);
    }

    const std::unordered_map<std::string, NineSliceTexture>& nine_slice::GetTextures()
    {
        return textures;
    }

    std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
    nine_slice::LoadTexture(Assets& assets, const std::string& id, const std::string& filename, const Recti& border_thickness, const Recti& area)
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

        auto tl0 = Vec2i::Zero();
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = rect.GetSize() - border_thickness.GetSize();
        auto tl3 = rect.GetSize();

        assets.LoadTexture(std::string("nine_slice_") + id + "_center",         filename, Recti(tl1, tl2 - tl1));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top_left",       filename, Recti(tl0, tl1));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top",            filename, Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top_right",      filename, Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_right",          filename, Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_right",   filename, Recti(tl2, tl3 - tl2));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom",         filename, Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_left",    filename, Recti(0, tl2.y, tl1.x, tl3.y - tl2.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_left",           filename, Recti(0, tl1.y, tl1.x, tl2.y - tl1.y));

        return result;
    }

    std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
    nine_slice::LoadTexture(Assets& assets, const std::string& id, void* data, std::size_t size, const Recti& border_thickness, const Recti& area)
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

        auto tl0 = Vec2i::Zero();
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = rect.GetSize() - border_thickness.GetSize();
        auto tl3 = rect.GetSize();

        assets.LoadTexture(std::string("nine_slice_") + id + "_center",         data, size, Recti(tl1, tl2 - tl1));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top_left",       data, size, Recti(tl0, tl1));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top",            data, size, Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top_right",      data, size, Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_right",          data, size, Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_right",   data, size, Recti(tl2, tl3 - tl2));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom",         data, size, Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_left",    data, size, Recti(0, tl2.y, tl1.x, tl3.y - tl2.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_left",           data, size, Recti(0, tl1.y, tl1.x, tl2.y - tl1.y));

        return result;
    }

    std::pair<std::unordered_map<std::string, NineSliceTexture>::iterator, bool>
    nine_slice::LoadTexture(Assets& assets, const std::string& id, const io::Image& image, const Recti& border_thickness, const Recti& area)
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

        auto tl0 = Vec2i::Zero();
        auto tl1 = border_thickness.GetPosition();
        auto tl2 = rect.GetSize() - border_thickness.GetSize();
        auto tl3 = rect.GetSize();

        assets.LoadTexture(std::string("nine_slice_") + id + "_center",         image, Recti(tl1, tl2 - tl1));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top_left",       image, Recti(tl0, tl1));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top",            image, Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_top_right",      image, Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_right",          image, Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_right",   image, Recti(tl2, tl3 - tl2));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom",         image, Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_bottom_left",    image, Recti(0, tl2.y, tl1.x, tl3.y - tl2.y));
        assets.LoadTexture(std::string("nine_slice_") + id + "_left",           image, Recti(0, tl1.y, tl1.x, tl2.y - tl1.y));

        return result;
    }

    void nine_slice::SetRenderOptions(Assets& assets, const std::string& id, TextureEx::RenderOptions options)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = assets.textures.at(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.SetRenderOptions(options);
        }
    }

    void nine_slice::SetSmooth(Assets& assets, const std::string& id, bool smooth)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = assets.textures.at(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.SetSmooth(smooth);
        }
    }

    bool nine_slice::IsSmooth(const Assets& assets, const std::string& id)
    {
        return assets.textures.at(TextureIDFromSlice(Texture::Slice::Center, id)).IsSmooth();
    }

    void nine_slice::SetSRGB(Assets& assets, const std::string& id, bool srgb)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = assets.textures.at(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.SetSRGB(srgb);
        }
    }

    bool nine_slice::IsSRGB(const Assets& assets, const std::string& id)
    {
        return assets.textures.at(TextureIDFromSlice(Texture::Slice::Center, id)).IsSRGB();
    }

    void nine_slice::SetRepeated(Assets& assets, const std::string& id, bool repeated)
    {
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            auto& texture = assets.textures.at(TextureIDFromSlice(static_cast<Texture::Slice>(i), id));

            texture.SetRepeated(repeated);
        }
    }

    bool nine_slice::IsRepeated(const Assets& assets, const std::string& id)
    {
        return assets.textures.at(TextureIDFromSlice(Texture::Slice::Center, id)).IsRepeated();
    }

    Component<CompoundSprite>& nine_slice::Add9SliceSprite(Assets& assets, Entity entity, const SpriteFactory& factory)
    {
        return Add9SliceSprite(assets, entity, factory.texture_id, factory.size, factory.center_texture_size, factory.border_thickness, factory.origin, factory.color);
    }

    Component<CompoundSprite>& nine_slice::Add9SliceSprite(
        Assets& assets,
        Entity entity,
        const std::string& texture_id,
        const Vec2f& size,
        const Vec2i& center_texture_size,
        const Rectf& border_thickness,
        const Vec2f& origin,
        const Color& color)
    {
        auto& comp_sprite = entity.AddComponent<CompoundSprite>();
        auto& sprites = comp_sprite.sprites;
        auto& slice_tex = textures.at(texture_id);
        
        auto rect_size = math::AutoSize(size, slice_tex.texture_rectangle.GetSize());
        auto bord_thick = border_thickness == Rectf() ? 
            Rectf(
                slice_tex.border_thickness.GetPosition() * rect_size / (slice_tex.texture_rectangle.GetSize() + center_texture_size), 
                slice_tex.border_thickness.GetSize() * rect_size / (slice_tex.texture_rectangle.GetSize() + center_texture_size)
            ) : 
            border_thickness;

        auto tl0 = Vec2f::Zero();
        auto tl1 = bord_thick.GetPosition();
        auto tl2 = rect_size - bord_thick.GetSize();
        auto tl3 = rect_size;

        sprites.resize(Texture::Slice::Count);

        sprites.at(Texture::Slice::Center)  .size = rect_size - bord_thick.GetPosition() - bord_thick.GetSize();

        sprites.at(Texture::Slice::Top)     .size = sprites.at(Texture::Slice::Center).size * Vec2f::i() + bord_thick.top     * Vec2f::j();
        sprites.at(Texture::Slice::Left)    .size = sprites.at(Texture::Slice::Center).size * Vec2f::j() + bord_thick.left    * Vec2f::i();
        sprites.at(Texture::Slice::Bottom)  .size = sprites.at(Texture::Slice::Center).size * Vec2f::i() + bord_thick.height  * Vec2f::j();
        sprites.at(Texture::Slice::Right)   .size = sprites.at(Texture::Slice::Center).size * Vec2f::j() + bord_thick.width   * Vec2f::i();

        sprites.at(Texture::Slice::TopLeft)     .size = Vec2f(bord_thick.left,    bord_thick.top);
        sprites.at(Texture::Slice::BottomLeft)  .size = Vec2f(bord_thick.left,    bord_thick.height);
        sprites.at(Texture::Slice::BottomRight) .size = Vec2f(bord_thick.width,   bord_thick.height);
        sprites.at(Texture::Slice::TopRight)    .size = Vec2f(bord_thick.width,   bord_thick.top);

        sprites.at(Texture::Slice::Center)  .texture_rectangle.SetSize(center_texture_size);

        sprites.at(Texture::Slice::Top)     .texture_rectangle.SetSize(sprites.at(Texture::Slice::Center).texture_rectangle.GetSize() * Vec2f::i());
        sprites.at(Texture::Slice::Left)    .texture_rectangle.SetSize(sprites.at(Texture::Slice::Center).texture_rectangle.GetSize() * Vec2f::j());
        sprites.at(Texture::Slice::Bottom)  .texture_rectangle.SetSize(sprites.at(Texture::Slice::Center).texture_rectangle.GetSize() * Vec2f::i());
        sprites.at(Texture::Slice::Right)   .texture_rectangle.SetSize(sprites.at(Texture::Slice::Center).texture_rectangle.GetSize() * Vec2f::j());

        sprites.at(Texture::Slice::Center)      .origin = origin - tl1;
        sprites.at(Texture::Slice::TopLeft)     .origin = origin - tl0;
        sprites.at(Texture::Slice::BottomLeft)  .origin = origin - Vec2f(0, tl2.y);
        sprites.at(Texture::Slice::BottomRight) .origin = origin - tl2;
        sprites.at(Texture::Slice::TopRight)    .origin = origin - Vec2f(tl2.x, 0);
        sprites.at(Texture::Slice::Top)         .origin = origin - Vec2f(tl1.x, 0);
        sprites.at(Texture::Slice::Left)        .origin = origin - Vec2f(0, tl1.y);
        sprites.at(Texture::Slice::Bottom)      .origin = origin - Vec2f(tl1.x, tl2.y);
        sprites.at(Texture::Slice::Right)       .origin = origin - Vec2f(tl2.x, tl1.y);
    
        for (auto i = 0; i < Texture::Slice::Count; ++i)
        {
            sprites.at(i).texture_id = slice_tex.textures.at(i);
            sprites.at(i).color = color;
        }

        return comp_sprite;
    }
}