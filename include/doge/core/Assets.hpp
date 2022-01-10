#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Rect.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>

namespace doge
{
    struct Engine;
    struct Entity;
    struct Texture;
    struct Prefab;
    struct Image;

    struct Assets
    {
        std::unordered_set<std::string> asset_paths = { "assets" };

        std::unordered_set<std::string> texture_sub_paths = { "textures" };
        std::unordered_map<std::string, Texture> textures; // require windows to be created

        std::unordered_map<std::string, std::function<Entity(Engine&)>> prefabs;

        void Clear();

        std::string SearchForAsset(const std::string& filename, const std::unordered_set<std::string>& sub_paths) const;

        std::string SearchForTexture(const std::string& filename) const;
        bool LoadTexture(const std::string& id, const std::string& filename, const Recti& area = Recti());
        bool LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& area = Recti());
        bool LoadTexture(const std::string& id, const Image& image, const Recti& area = Recti());
    };
}