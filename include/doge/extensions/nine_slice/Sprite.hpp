#pragma once

#include <array>
#include <string>
#include <doge/components/Sprite.hpp>
#include <doge/extensions/nine_slice/Slice.hpp>

namespace doge::nine_slice
{
    struct NineSlice;

    struct Sprite
    {
        enum BorderThickness
        {
            Unchange,
            Stretch,
            TileScale,
            HorizontalScale,
            VerticalScale,
        };

        std::string texture_id = "missing_texture";
        Vec2f size = Vec2f::Zero;
        Vec2i center_texture_size = Vec2f::Zero;
        Rectf border_thickness = Rectf();

        Vec2f origin = Vec2f::Zero;
        Color color = Color::White;

        // auto generated
        std::array<doge::Sprite, Slice::Count> sprites;
        
        static Sprite CreateByTile(
            const NineSlice& nine_slice,
            const std::string& texture_id,
            const Vec2f& tile_size,
            const Vec2u& tile_count = Vec2u::One,
            BorderThickness border_thickness = BorderThickness::Stretch,
            const Vec2f& origin = Vec2f::Zero,
            const Color& color = Color::White
        );
    };
}