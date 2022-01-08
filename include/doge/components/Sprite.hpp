#pragma once

#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>
#include <doge/core/Texture.hpp>

namespace doge
{
    struct Sprite
    {
        std::string texture_id = "missing_texture";
        Recti texture_rectangle = Recti();
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
    };
}