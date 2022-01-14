#include <doge/core/Assets.hpp>

#include <filesystem>
#include <doge/core.hpp>
#include <doge/utils.hpp>

namespace doge
{
    void Assets::Clear()
    {
        textures.clear();
        
        sound_buffers.clear();
        sounds.clear();
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

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    Assets::LoadTexture(const std::string& id, const std::string& filename, const Recti& area)
    {
        Texture texture;
        auto path = SearchForTexture(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return std::make_pair(textures.end(), false);
        }

        if (texture.FromFile(path, area))
            return textures.emplace(id, texture);
        
        return std::make_pair(textures.end(), false);
    }

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    Assets::LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& area)
    {
        Texture texture;

        if (texture.FromMemory(data, size, area))
            return textures.emplace(id, texture);

        return std::make_pair(textures.end(), false);
    }

    std::pair<std::unordered_map<std::string, Texture>::iterator, bool>
    Assets::LoadTexture(const std::string& id, const Image& image, const Recti& area)
    {
        Texture texture;

        if (texture.FromImage(image, area))
            return textures.emplace(id, texture);
        
        return std::make_pair(textures.end(), false);
    }

    std::string Assets::SearchForSound(const std::string& filename) const
    {
        return SearchForAsset(filename, sound_sub_paths);
    }

    std::pair<std::unordered_map<std::string, io::SoundBuffer>::iterator, bool>
    Assets::LoadSoundBuffer(const std::string& id, const std::string& filename)
    {
        io::SoundBuffer buffer;
        auto path = SearchForSound(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return std::make_pair(sound_buffers.end(), false);
        }

        if (buffer.FromFile(path))
            return sound_buffers.emplace(id, buffer);
        
        return std::make_pair(sound_buffers.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::SoundBuffer>::iterator, bool>
    Assets::LoadSoundBuffer(const std::string& id, void* data, std::size_t size)
    {
        io::SoundBuffer buffer;

        if (buffer.FromMemory(data, size))
            return sound_buffers.emplace(id, buffer);

        return std::make_pair(sound_buffers.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::SoundBuffer>::iterator, bool>
    Assets::LoadSoundBuffer(
        const std::string& id, 
        const std::int16_t* samples, 
        std::uint64_t sample_count, 
        std::uint32_t channel_count, 
        std::uint32_t sample_rate
        )
    {
        io::SoundBuffer buffer;

        if (buffer.FromSamples(samples, sample_count, channel_count, sample_rate))
            return sound_buffers.emplace(id, buffer);

        return std::make_pair(sound_buffers.end(), false);
    }
    
    std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
    Assets::AddSound(const std::string& id, const std::string& buffer_id)
    {
        return sounds.emplace(id, io::Sound(sound_buffers.at(buffer_id)));
    }

    std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
    Assets::AddSound(const std::string& id, const io::SoundBuffer& buffer)
    {
        return sounds.emplace(id, io::Sound(buffer));
    }

    std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
    Assets::AddSound(const std::string& id, const std::string& buffer_id, const std::string& filename)
    {
        auto [itr, success] = LoadSoundBuffer(buffer_id, filename);
        if (!success)
        {
            std::cerr << "Unable to load SoundBuffer" << std::endl;
            return std::make_pair(sounds.end(), false);
        }

        return sounds.emplace(id, io::Sound(itr->second));
    }

    std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
    Assets::AddSound(const std::string& id, const std::string& buffer_id, void* data, std::size_t size)
    {
        auto [itr, success] = LoadSoundBuffer(buffer_id, data, size);
        if (!success)
        {
            std::cerr << "Unable to load SoundBuffer" << std::endl;
            return std::make_pair(sounds.end(), false);
        }

        return sounds.emplace(id, io::Sound(itr->second));
    }

    std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
    Assets::AddSound(
        const std::string& id, 
        const std::string& buffer_id, 
        const std::int16_t* samples, 
        std::uint64_t sample_count, 
        std::uint32_t channel_count, 
        std::uint32_t sample_rate
        )
    {
        auto [itr, success] = LoadSoundBuffer(buffer_id, samples, sample_count, channel_count, sample_rate);
        if (!success)
        {
            std::cerr << "Unable to load SoundBuffer" << std::endl;
            return std::make_pair(sounds.end(), false);
        }

        return sounds.emplace(id, io::Sound(itr->second));
    }
}