#include <doge/core/Assets.hpp>

#include <filesystem>
#include <doge/core.hpp>
#include <doge/utils.hpp>

namespace doge
{
    std::unordered_set<std::string> Assets::asset_paths = { "assets" };
    std::unordered_set<std::string> Assets::font_sub_paths = { "fonts" };
    std::unordered_set<std::string> Assets::texture_sub_paths = { "textures" };
    std::unordered_set<std::string> Assets::image_sub_paths = { "images" };
    std::unordered_set<std::string> Assets::cursor_sub_paths = { "cursors" };
    std::unordered_set<std::string> Assets::sound_sub_paths = { "sounds" };
    std::unordered_set<std::string> Assets::music_sub_paths = { "musics " };

    std::string Assets::SearchForAsset(const std::string& filename, const std::unordered_set<std::string>& sub_paths)
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

    std::string Assets::SearchForFont(const std::string& filename)
    {
        return SearchForAsset(filename, font_sub_paths);
    }

    std::pair<std::unordered_map<std::string, io::Font>::iterator, bool>
    Assets::LoadFont(const std::string& id, const std::string& filename)
    {
        io::Font font;
        auto path = SearchForFont(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return std::make_pair(fonts.end(), false);
        }

        if (font.FromFile(path))
            return fonts.emplace(id, font);
        
        return std::make_pair(fonts.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::Font>::iterator, bool>
    Assets::LoadFont(const std::string& id, void* data, std::size_t size)
    {
        io::Font font;

        if (font.FromMemory(data, size))
            return fonts.emplace(id, font);

        return std::make_pair(fonts.end(), false);
    }

    std::string Assets::SearchForTexture(const std::string& filename)
    {
        return SearchForAsset(filename, texture_sub_paths);
    }

    std::pair<std::unordered_map<std::string, TextureEx>::iterator, bool>
    Assets::LoadTexture(const std::string& id, const std::string& filename, const Recti& area)
    {
        io::Texture texture;
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

    std::pair<std::unordered_map<std::string, TextureEx>::iterator, bool>
    Assets::LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& area)
    {
        io::Texture texture;

        if (texture.FromMemory(data, size, area))
            return textures.emplace(id, texture);

        return std::make_pair(textures.end(), false);
    }

    std::pair<std::unordered_map<std::string, TextureEx>::iterator, bool>
    Assets::LoadTexture(const std::string& id, const io::Image& image, const Recti& area)
    {
        io::Texture texture;

        if (texture.FromImage(image, area))
            return textures.emplace(id, texture);
        
        return std::make_pair(textures.end(), false);
    }

    std::string Assets::SearchForImage(const std::string& filename)
    {
        return SearchForAsset(filename, image_sub_paths);
    }

    std::pair<std::unordered_map<std::string, io::Image>::iterator, bool>
    Assets::LoadImage(const std::string& id, const std::string& filename)
    {
        io::Image image;
        auto path = SearchForImage(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return std::make_pair(images.end(), false);
        }

        if (image.FromFile(path))
            return images.emplace(id, image);
        
        return std::make_pair(images.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::Image>::iterator, bool>
    Assets::LoadImage(const std::string& id, void* data, std::size_t size)
    {
        io::Image image;

        if (image.FromMemory(data, size))
            return images.emplace(id, image);

        return std::make_pair(images.end(), false);
    }

    std::string Assets::SearchForCursor(const std::string& filename)
    {
        return SearchForAsset(filename, cursor_sub_paths);
    }

    std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
    Assets::LoadCursor(const std::string& id, const std::string& filename, const Vec2u& hotspot)
    {
        io::Cursor cursor;
        auto path = SearchForImage(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return std::make_pair(cursors.end(), false);
        }

        auto p = cursors.try_emplace(id);

        if (!p.second || p.first->second.FromFile(path))
            return p;
        
        return std::make_pair(cursors.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
    Assets::LoadCursor(const std::string& id, const io::Image& image, const Vec2u& hotspot)
    {
        io::Cursor cursor;

        auto p = cursors.try_emplace(id);

        if (!p.second || p.first->second.FromImage(image, hotspot))
            return p;

        return std::make_pair(cursors.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
    Assets::LoadCursor(const std::string& id, const std::uint8_t* pixels, const Vec2u& size, const Vec2u& hotspot)
    {
        io::Cursor cursor;

        auto p = cursors.try_emplace(id);

        if (!p.second || p.first->second.FromPixels(pixels, size, hotspot))
            return p;

        return std::make_pair(cursors.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
    Assets::LoadCursor(const std::string& id, io::Cursor::Type type)
    {
        io::Cursor cursor;

        auto p = cursors.try_emplace(id);

        if (!p.second || p.first->second.FromSystem(type))
            return p;

        return std::make_pair(cursors.end(), false);
    }

    std::string Assets::SearchForSound(const std::string& filename)
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

    std::string Assets::SearchForMusic(const std::string& filename)
    {
        return SearchForAsset(filename, music_sub_paths);
    }

    std::pair<std::unordered_map<std::string, io::Music>::iterator, bool>
    Assets::LoadMusic(const std::string& id, const std::string& filename)
    {
        auto path = SearchForMusic(filename);

        if (path == "")
        {
            std::cerr << "Cannot find path: " << filename << std::endl;
            return std::make_pair(musics.end(), false);
        }

        auto p = musics.try_emplace(id);

        if (!p.second || p.first->second.FromFile(path))
            return p;
        
        return std::make_pair(musics.end(), false);
    }

    std::pair<std::unordered_map<std::string, io::Music>::iterator, bool>
    Assets::LoadMusic(const std::string& id, void* data, std::size_t size)
    {
        auto p = musics.try_emplace(id);

        if (!p.second || p.first->second.FromMemory(data, size))
            return p;

        return std::make_pair(musics.end(), false);
    }
}