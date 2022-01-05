#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/core/Texture.hpp>
#include <unordered_set>

namespace doge
{
    class AssetManager
    {
        std::unordered_set<Texture> textures;
    };
}