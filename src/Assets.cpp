#include <doge/core/Assets.hpp>

#include <filesystem>

namespace doge
{
    void Assets::Clear()
    {
        textures.clear();
    }

    bool Assets::LoadTexture(const std::string& id, const std::string& filename)
    {
        Texture texture;

        auto SearchForFile = [&]() -> std::string
        {
            if (std::filesystem::exists(filename))
                return filename;

            for (auto& path : asset_paths)
            {
                if (std::filesystem::exists(path + "/" + filename))
                    return path + "/" + filename;
                
                for (auto& sub_path : texture_sub_paths)
                {
                    if (std::filesystem::exists(path + "/" + sub_path + "/" + filename))
                        return path + "/" + sub_path + "/" + filename;
                }
            }

            return "";
        };

        auto path = SearchForFile();

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return false;
        }

        if (texture.FromFile(path))
            return textures.emplace(id, texture).second;
        
        return false;
    }
}