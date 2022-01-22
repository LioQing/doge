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
    };
}