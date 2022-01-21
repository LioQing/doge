#pragma once

#include <doge/utils/Color.hpp>
#include <doge/utils/Vec2.hpp>
#include <string>

namespace doge
{
    struct NineSliceSpriteFactory
    {
        std::string texture_id = "missing_texture";
        Vec2f size;
        Vec2i center_texture_size;
        Rectf border_thickness;
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
    };
}