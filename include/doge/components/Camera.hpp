#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/aliases.hpp>
#include <unordered_set>

namespace doge
{
    struct Camera
    {
        // size of camera in scene
        Vec2f size = Vec2f::Zero; // (0, 0) represent auto resize corresponding to port rect
        
        // size of camera in window (in ratio to the window)
        Rectf port = Rectf(0, 0, 1, 1);

        std::int32_t render_order = 0;
    };
}