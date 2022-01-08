#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/core/Texture.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>

namespace doge
{
    struct Assets
    {
        std::unordered_set<std::string> asset_paths = { "assets" };

        std::unordered_set<std::string> texture_sub_paths = { "textures" };
        std::unordered_map<std::string, Texture> textures; // require windows to be created

        void Clear();

        bool LoadTexture(const std::string& id, const std::string& filename);
    };
}