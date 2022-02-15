#pragma once

#include <array>
#include <string>
#include <doge/utils/Rect.hpp>
#include <doge/extensions/nine_slice/Slice.hpp>

namespace doge::nine_slice
{
    struct Texture
    {
        std::array<std::string, Slice::Count> textures;
        Recti texture_rectangle;
        Recti border_thickness;

        static std::string PostFixFromSlice(Slice slice);
        static Slice SliceFromPostFix(const std::string& post_fix);
    };
}