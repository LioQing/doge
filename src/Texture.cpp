#include <doge/core/Texture.hpp>

#include <doge/core/Image.hpp>
#include <cassert>

namespace doge
{
    Texture::SpriteSheet::Type Texture::SpriteSheet::GetType() const
    {
        return type;
    }

    void Texture::SpriteSheet::SetNone()
    {
        type = SpriteSheet::Type::None;
        frame = nullptr;
    }

    void Texture::SpriteSheet::SetVariedSize(const std::vector<Recti>& frames)
    {
        type = SpriteSheet::Type::VariedSize;
        frame = frames;
    }

    void Texture::SpriteSheet::SetFixedSize(const Vec2u& size)
    {
        type = SpriteSheet::Type::FixedSize;
        frame = size;
    }

    Recti Texture::SpriteSheet::GetFrame(std::size_t index) const
    {
        assert(type == SpriteSheet::Type::VariedSize && "Texture has the wrong SpriteSheet::Type when calling SpriteSheet::GetFrame(std::size_t index)");
        return std::get<std::vector<Recti>>(frame).at(index);
    }

    Recti Texture::SpriteSheet::GetFrame(const Vec2i& indices) const
    {
        assert(type == SpriteSheet::Type::FixedSize && "Texture has the wrong SpriteSheet::Type when calling SpriteSheet::GetFrame(const Vec2i& indices)");
        auto& size = std::get<Vec2u>(frame);
        return Recti(size.x * indices.x, size.y * indices.y, size.x, size.y);
    }

    Texture::RenderOptions::RenderOptions(IOBus::TextureData& texture_data) : texture_data(texture_data)
    {
    }

    void Texture::RenderOptions::Set(Options options)
    {
        texture_data.SetSmooth(options & Options::Smooth);
        texture_data.SetSRGB(options & Options::sRGB);
        texture_data.SetRepeated(options & Options::Repeated);
    }

    bool Texture::RenderOptions::IsSmooth() const
    {
        return texture_data.IsSmooth();
    }

    bool Texture::RenderOptions::IsSRGB() const
    {
        return texture_data.IsSRGB();
    }

    bool Texture::RenderOptions::IsRepeated() const
    {
        return texture_data.IsRepeated();
    }

    Texture::Texture(const IOBus::TextureData& texture_data) : texture_data(texture_data)
    {
    }

    Texture::Texture(const Vec2u& size)
    {
        texture_data.Create(size);
    }
    
    Texture::Texture(const std::string& filename, const Recti& area)
    {
        texture_data.FromFile(filename, area);
    }

    Texture::Texture(void* data, std::size_t size, const Recti& area)
    {
        texture_data.FromMemory(data, size, area);
    }

    Texture::Texture(const Image& image, const Recti& area)
    {
        texture_data.FromImage(image.image_data, area);
    }
}