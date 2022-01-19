#pragma once

#include <doge/utils/Color.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct CircleShape
    {
        float radius = 1.f;
        std::size_t point_count = 32;
        Vec2f origin = Vec2f::Zero();
        Color color = Color::White();
        Color outline_color = Color::White();
        float outline_thickness = 0.f;
        std::string texture_id = "";
        std::string atlas_rectangle_id = "";
        Recti texture_rectangle = Recti();
    };
}