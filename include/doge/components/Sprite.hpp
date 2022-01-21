#pragma once

#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    struct Sprite
    {
        std::string texture_id = "missing_texture";
        std::string atlas_rectangle_id = "";
        Recti texture_rectangle = Recti();
        Vec2f size = Vec2f::Zero();
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
    };
}