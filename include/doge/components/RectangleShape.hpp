#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    struct RectangleShape
    {
        Vec2f size = Vec2f::One();
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
        Color outline_color = Color::White();
        float outline_thickness = 0.f;
        std::string texture_id = "";
        Recti texture_rectangle = Recti();
    };
}