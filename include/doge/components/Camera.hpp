#pragma once

#include "../utils/Vec2.hpp"
#include "../utils/Rect.hpp"

namespace doge
{
    struct Camera
    {
        // size of camera in scene
        Vec2f size = Vec2f::Zero(); // (0, 0) represent auto resize corresponding to port rect
        
        // size of camera in window (in pixels)
        Rectu port = Rectu(0, 0, 0, 0); // (0, 0, 0, 0) represent auto resize corresponding to window
    };
}