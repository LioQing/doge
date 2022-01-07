#pragma once

#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>
#include <doge/core/Texture.hpp>

namespace doge
{
    struct Sprite
    {
        Texture texture;
        Recti texture_rectangle;
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
    };
}