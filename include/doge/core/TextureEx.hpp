#pragma once

#include <doge/core/io/Texture.hpp>
#include <doge/utils/Rect.hpp>
#include <unordered_map>
#include <string>

namespace doge
{
    struct TextureEx : io::Texture
    {
        std::unordered_map<std::string, Recti> atlas_rectangles;
    };
}