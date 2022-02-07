#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/core/io.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <functional>

namespace doge
{
    struct Engine;
    struct Entity;
    struct Texture;

    struct Assets
    {
        static std::unordered_set<std::string> asset_paths;
        static std::unordered_set<std::string> font_sub_paths;
        static std::unordered_set<std::string> texture_sub_paths;
        static std::unordered_set<std::string> image_sub_paths;
        static std::unordered_set<std::string> cursor_sub_paths;
        static std::unordered_set<std::string> sound_sub_paths;
        static std::unordered_set<std::string> music_sub_paths;

    private:

        std::unordered_map<std::string, io::Font> fonts;
        std::unordered_map<std::string, io::Texture> textures; // require windows to be created
        std::unordered_map<std::string, io::Image> images;
        std::unordered_map<std::string, io::Cursor> cursors;
        std::unordered_map<std::string, io::SoundBuffer> sound_buffers;
        std::unordered_map<std::string, io::Sound> sounds;
        std::unordered_map<std::string, io::Music> musics;

    public:

        static std::string SearchForAsset(const std::string& filename, const std::unordered_set<std::string>& sub_paths);

        static std::string SearchForFont(const std::string& filename);

        io::Font& GetFont(const std::string& id);
        const io::Font& GetFont(const std::string& id) const;
        void EraseFont(const std::string& id);
        bool HasFont(const std::string& id) const;

        std::pair<std::unordered_map<std::string, io::Font>::iterator, bool>
        LoadFont(const std::string& id, const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Font>::iterator, bool>
        LoadFont(const std::string& id, void* data, std::size_t size);

        static std::string SearchForTexture(const std::string& filename);

        io::Texture& GetTexture(const std::string& id);
        const io::Texture& GetTexture(const std::string& id) const;
        void EraseTexture(const std::string& id);
        bool HasTexture(const std::string& id) const;

        std::pair<std::unordered_map<std::string, io::Texture>::iterator, bool>
        LoadTexture(const std::string& id, const std::string& filename, const Recti& area = Recti());

        std::pair<std::unordered_map<std::string, io::Texture>::iterator, bool>
        LoadTexture(const std::string& id, void* data, std::size_t size, const Recti& area = Recti());

        std::pair<std::unordered_map<std::string, io::Texture>::iterator, bool>
        LoadTexture(const std::string& id, const io::Image& image, const Recti& area = Recti());

        static std::string SearchForImage(const std::string& filename);

        io::Image& GetImage(const std::string& id);
        const io::Image& GetImage(const std::string& id) const;
        void EraseImage(const std::string& id);
        bool HasImage(const std::string& id) const;

        std::pair<std::unordered_map<std::string, io::Image>::iterator, bool>
        LoadImage(const std::string& id, const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Image>::iterator, bool>
        LoadImage(const std::string& id, void* data, std::size_t size);

        static std::string SearchForCursor(const std::string& filename);

        io::Cursor& GetCursor(const std::string& id);
        const io::Cursor& GetCursor(const std::string& id) const;
        void EraseCursor(const std::string& id);
        bool HasCursor(const std::string& id) const;

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, const std::string& filename, const Vec2u& hotspot);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, const io::Image& image, const Vec2u& hotspot);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, const std::uint8_t* pixels, const Vec2u& size, const Vec2u& hotspot);

        std::pair<std::unordered_map<std::string, io::Cursor>::iterator, bool>
        LoadCursor(const std::string& id, io::Cursor::Type type);

        static std::string SearchForSound(const std::string& filename);

        io::SoundBuffer& GetSoundBuffer(const std::string& id);
        const io::SoundBuffer& GetSoundBuffer(const std::string& id) const;
        void EraseSoundBuffer(const std::string& id);
        bool HasSoundBuffer(const std::string& id) const;

        io::Sound& GetSound(const std::string& id);
        const io::Sound& GetSound(const std::string& id) const;
        void EraseSound(const std::string& id);
        bool HasSound(const std::string& id) const;

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

        io::Music& GetMusic(const std::string& id);
        const io::Music& GetMusic(const std::string& id) const;
        void EraseMusic(const std::string& id);
        bool HasMusic(const std::string& id) const;

        std::pair<std::unordered_map<std::string, io::Music>::iterator, bool>
        LoadMusic(const std::string& id, const std::string& filename);

        std::pair<std::unordered_map<std::string, io::Music>::iterator, bool>
        LoadMusic(const std::string& id, void* data, std::size_t size);
    };
}