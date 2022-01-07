#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/core/Texture.hpp>
#include <unordered_map>
#include <string>

namespace doge
{
    struct Assets
    {
        std::unordered_map<std::string, Texture> textures;
    };
}