#include <doge/core/Assets.hpp>

#include <filesystem>
#include <doge/core.hpp>
#include <doge/utils.hpp>

namespace doge
{
    void Assets::Clear()
    {
        textures.clear();
        prefabs.clear();
    }

    std::string Assets::SearchForAsset(const std::string& filename, const std::unordered_set<std::string>& sub_paths) const
    {
        if (std::filesystem::exists(filename))
            return filename;

        for (auto& path : asset_paths)
        {
            if (std::filesystem::exists(path + "/" + filename))
                return path + "/" + filename;
            
            for (auto& sub_path : sub_paths)
            {
                if (std::filesystem::exists(path + "/" + sub_path + "/" + filename))
                    return path + "/" + sub_path + "/" + filename;
            }
        }

        return "";
    }

    std::string Assets::SearchForTexture(const std::string& filename) const
    {
        return SearchForAsset(filename, texture_sub_paths);
    }

    bool Assets::LoadTexture(const std::string& id, const std::string& filename, const Recti& area)
    {
        Texture texture;
        auto path = SearchForTexture(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return false;
        }

        if (texture.FromFile(path, area))
            return textures.emplace(id, texture).second;
        
        return false;
    }

    bool Assets::LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& area)
    {
        Texture texture;

        if (texture.FromMemory(data, size, area))
            return textures.emplace(id, texture).second;

        return false;
    }

    bool Assets::LoadTexture(const std::string& id, const Image& image, const Recti& area)
    {
        Texture texture;

        if (texture.FromImage(image, area))
            return textures.emplace(id, texture).second;
        
        return false;
    }
}