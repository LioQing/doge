#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io/Cursor.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>

namespace doge
{
    struct Engine;
    struct Entity;
    struct TextureEx;

    namespace io
    {
        struct Image;
        struct Texture;
        struct Cursor;
        struct SoundBuffer;
        struct Sound;
        struct Music;
    }

    struct Assets
    {
        static std::unordered_set<std::string> asset_paths;

        static std::unordered_set<std::string> texture_sub_paths;
        std::unordered_map<std::string, TextureEx> textures; // require windows to be created
        
        static std::unordered_set<std::string> image_sub_paths;
        std::unordered_map<std::string, io::Image> images;
        
        static std::unordered_set<std::string> cursor_sub_paths;
        std::unordered_map<std::string, io::Cursor> cursors;

        static std::unordered_set<std::string> sound_sub_paths;
        std::unordered_map<std::string, io::SoundBuffer> sound_buffers;
        std::unordered_map<std::string, io::Sound> sounds;

        static std::unordered_set<std::string> music_sub_paths;
        std::unordered_map<std::string, io::Music> musics;

        static std::string SearchForAsset(const std::string& filename, const std::unordered_set<std::string>& sub_paths);

        static std::string SearchForTexture(const std::string& filename);

        std::pair<std::unordered_map<std::string, TextureEx>::iterator, bool>
        LoadTexture(const std::string& id, const std::string& filename, const Recti& area = Recti());

        std::pair<std::unordered_map<std::string, TextureEx>::iterator, bool>
        LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& area = Recti());

        std::pair<std::unordered_map<std::string, TextureEx>::iterator, bool>
        LoadTexture(const std::string& id, const io::Image& image, const Recti& area = Recti());

        static std::string SearchForImage(const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Image>::iterator, bool>
        LoadImage(const std::string& id, const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Image>::iterator, bool>
        LoadImage(const std::string& id, void* data, std::size_t size);

        static std::string SearchForCursor(const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, const std::string& filename, const Vec2u& hotspot);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, const io::Image& image, const Vec2u& hotspot);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, const std::uint8_t* pixels, const Vec2u& size, const Vec2u& hotspot);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, io::Cursor::Type type);

        static std::string SearchForSound(const std::string& filename);

        std::pair<std::unordered_map<std::string, io::SoundBuffer>::iterator, bool>
        LoadSoundBuffer(const std::string& id, const std::string& filename);

        std::pair<std::unordered_map<std::string, io::SoundBuffer>::iterator, bool>
        LoadSoundBuffer(const std::string& id, void* data, std::size_t size);

        std::pair<std::unordered_map<std::string, io::SoundBuffer>::iterator, bool>
        LoadSoundBuffer(
            const std::string& id, 
            const std::int16_t* samples, 
            std::uint64_t sample_count, 
            std::uint32_t channel_count, 
            std::uint32_t sample_rate
            );

        std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
        AddSound(const std::string& id, const std::string& buffer_id);

        std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
        AddSound(const std::string& id, const io::SoundBuffer& buffer);
        
        std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
        AddSound(const std::string& id, const std::string& buffer_id, const std::string& filename);
        
        std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
        AddSound(const std::string& id, const std::string& buffer_id, void* data, std::size_t size);
        
        std::pair<std::unordered_map<std::string, io::Sound>::iterator, bool>
        AddSound(
            const std::string& id, 
            const std::string& buffer_id, 
            const std::int16_t* samples, 
            std::uint64_t sample_count, 
            std::uint32_t channel_count, 
            std::uint32_t sample_rate
            );

        static std::string SearchForMusic(const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Music>::iterator, bool>
        LoadMusic(const std::string& id, const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Music>::iterator, bool>
        LoadMusic(const std::string& id, void* data, std::size_t size);
    };
}