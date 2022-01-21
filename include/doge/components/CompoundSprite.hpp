#pragma once

#include <vector>
#include <doge/core/io/Texture.hpp>
#include <doge/components/ConvexShape.hpp>
#include <doge/components/CircleShape.hpp>
#include <doge/components/RectangleShape.hpp>
#include <doge/components/Sprite.hpp>

namespace doge
{
    struct CompoundSprite
    {
        std::vector<ConvexShape> convex_shapes;
        std::vector<CircleShape> circle_shapes;
        std::vector<RectangleShape> rectangle_shapes;
        std::vector<Sprite> sprites;

        template <typename... Sprites>
        static CompoundSprite Create(Sprites&&... sprites)
        {
            CompoundSprite comp_sprite;

            auto emplace_sprite =
            [&]<typename S>(const S& sprite)
            {
                if constexpr (std::is_constructible_v<S, ConvexShape>)
                    comp_sprite.convex_shapes.emplace_back(sprite);
                else if constexpr (std::is_constructible_v<S, CircleShape>)
                    comp_sprite.circle_shapes.emplace_back(sprite);
                else if constexpr (std::is_constructible_v<S, RectangleShape>)
                    comp_sprite.rectangle_shapes.emplace_back(sprite);
                else if constexpr (std::is_constructible_v<S, Sprite>)
                    comp_sprite.sprites.emplace_back(sprite);
                else
                    static_assert(false && "Invalid Sprite Type");
            };

            (emplace_sprite(sprites), ...);

            return comp_sprite;
        }

        // static CompoundSprite Create9SlicedSprite(
        //     const std::string& texture_id,
        //     const std::string& center_texture_id,
        //     const Vec2f& size,
        //     const Recti& texture_rectangle,
        //     const Recti& border_width,
        //     const Vec2f& origin = Vec2f::Zero(),
        //     const Color& color = Color::White())
        // {
        //     auto tl0 = Vec2i::Zero();
        //     auto tl1 = border_width.GetPosition();
        //     auto tl2 = texture_rectangle.GetSize() - border_width.GetSize();
        //     auto tl3 = texture_rectangle.GetSize();

        //     std::vector<Sprite> sprites = 
        //     {
        //         Sprite{ .texture_id = center_texture_id, .texture_rectangle = Recti(Vec2i::Zero(), tl2 - tl1), .origin = origin - tl1, .color = color },                            // center

        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(tl0, tl1),                          .origin = origin,                   .color = color },                 // top-left
        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(0, tl2.y, tl1.x, tl3.y - tl2.y),    .origin = origin - Vec2f(0, tl2.y), .color = color },                 // bottom-left
        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(tl2, tl3 - tl2),                    .origin = origin - tl2,             .color = color },                 // bottom-right
        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(tl2.x, 0, tl3.x - tl2.x, tl1.y),    .origin = origin - Vec2f(tl2.x, 0), .color = color },                 // top-right

        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(tl1.x, 0, tl2.x - tl1.x, tl1.y),                .origin = origin - Vec2f(tl1.x, 0), .color = color },     // top
        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(0, tl1.y, tl1.x, tl2.y - tl1.y),                .origin = origin - Vec2f(0, tl1.y), .color = color },     // left
        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(tl1.x, tl2.y, tl2.x - tl1.x, tl3.y - tl2.y),    .origin = origin - Vec2f(tl1.x, tl2.y), .color = color }, // bottom
        //         Sprite{ .texture_id = texture_id, .texture_rectangle = Recti(tl2.x, tl1.y, tl3.x - tl2.x, tl2.y - tl1.y),    .origin = origin - Vec2f(tl2.x, tl1.y), .color = color }, // right
        //     };

        //     if (size == Vec2f::Zero())
        //         return CompoundSprite{ .sprites = sprites };

        //     auto center_scale = 
        //     sprites.at(0).size = size * sprites.at(0).texture_rectangle.GetSize() / tl3;
            
        //     sprites.at(5).size = size * sprites.at(5).texture_rectangle.GetSize() / tl3 * Vec2f::i();
        //     sprites.at(6).size = size * sprites.at(6).texture_rectangle.GetSize() / tl3 * Vec2f::j();
        //     sprites.at(7).size = size * sprites.at(7).texture_rectangle.GetSize() / tl3 * Vec2f::i();
        //     sprites.at(8).size = size * sprites.at(8).texture_rectangle.GetSize() / tl3 * Vec2f::j();

        //     auto ctl = tl1 - origin 

        //     return CompoundSprite{ .sprites = sprites };
        // }
    };
}