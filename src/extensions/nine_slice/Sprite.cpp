#include <doge/extensions/nine_slice/Sprite.hpp>

#include <doge/extensions/nine_slice/NineSlice.hpp>

namespace doge::nine_slice
{    
    Sprite Sprite::CreateByTile(
        const NineSlice& nine_slice,
        const std::string& texture_id,
        const Vec2f& tile_size,
        const Vec2u& tile_count,
        BorderThickness border_thickness_option,
        const Vec2f& origin,
        const Color& color
    )
    {
        auto sprite = Sprite
        {
            .texture_id = texture_id,
            .origin = origin,
            .color = color,
        };
        auto& slice_tex = nine_slice.GetTextures().at(texture_id);

        auto texture_center_size = nine_slice.GetSliceTexture(texture_id, Slice::Center).GetSize();
        sprite.center_texture_size = texture_center_size * tile_count;

        sprite.size = tile_size * tile_count;

        sprite.border_thickness = slice_tex.border_thickness;
        if (border_thickness_option == Sprite::BorderThickness::Stretch)
        {
            sprite.border_thickness.SetPosition(sprite.border_thickness.GetPosition() * sprite.size / slice_tex.texture_rectangle.GetSize());
            sprite.border_thickness.SetSize(sprite.border_thickness.GetSize() * sprite.size / slice_tex.texture_rectangle.GetSize());
        }
        else if (border_thickness_option == Sprite::BorderThickness::TileScale)
        {
            sprite.border_thickness.SetPosition(sprite.border_thickness.GetPosition() * tile_size / slice_tex.texture_rectangle.GetSize());
            sprite.border_thickness.SetSize(sprite.border_thickness.GetSize() * tile_size / slice_tex.texture_rectangle.GetSize());
        }
        else if (border_thickness_option == Sprite::BorderThickness::HorizontalScale)
        {
            sprite.border_thickness.SetPosition(sprite.border_thickness.GetPosition() * sprite.size.x / slice_tex.texture_rectangle.GetSize().x);
            sprite.border_thickness.SetSize(sprite.border_thickness.GetSize() * sprite.size.x / slice_tex.texture_rectangle.GetSize().x);
        }
        else if (border_thickness_option == Sprite::BorderThickness::VerticalScale)
        {
            sprite.border_thickness.SetPosition(sprite.border_thickness.GetPosition() * sprite.size.y / slice_tex.texture_rectangle.GetSize().y);
            sprite.border_thickness.SetSize(sprite.border_thickness.GetSize() * sprite.size.y / slice_tex.texture_rectangle.GetSize().y);
        }

        return sprite;
    }
}