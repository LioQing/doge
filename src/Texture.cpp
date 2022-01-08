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

    Texture::Texture(const IOBus::TextureData& texture_data) : texture_data(texture_data)
    {
    }

    bool Texture::Create(const Vec2u& size)
    {
        return texture_data.Create(size);
    }
    
    bool Texture::FromFile(const std::string& filename, const Recti& area)
    {
        return texture_data.FromFile(filename, area);
    }

    bool Texture::FromMemory(void* data, std::size_t size, const Recti& area)
    {
        return texture_data.FromMemory(data, size, area);
    }

    bool Texture::FromImage(const Image& image, const Recti& area)
    {
        return texture_data.FromImage(image.image_data, area);
    }
    
    void Texture::SetRenderOptions(RenderOptions options)
    {
        texture_data.SetSmooth(options & RenderOptions::Smooth);
        texture_data.SetSRGB(options & RenderOptions::sRGB);
        texture_data.SetRepeated(options & RenderOptions::Repeated);
    }

    bool Texture::IsSmooth() const
    {
        return texture_data.IsSmooth();
    }

    bool Texture::IsSRGB() const
    {
        return texture_data.IsSRGB();
    }

    bool Texture::IsRepeated() const
    {
        return texture_data.IsRepeated();
    }

    Vec2u Texture::GetSize() const
    {
        return texture_data.GetSize();
    }
}