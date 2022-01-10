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
        if (type == SpriteSheet::Type::VariedSize)
            throw std::invalid_argument("Texture has the wrong SpriteSheet::Type when calling SpriteSheet::GetFrame(std::size_t index)");
        return std::get<std::vector<Recti>>(frame).at(index);
    }

    Recti Texture::SpriteSheet::GetFrame(const Vec2i& indices) const
    {
        if (type == SpriteSheet::Type::FixedSize)
            throw std::invalid_argument("Texture has the wrong SpriteSheet::Type when calling SpriteSheet::GetFrame(const Vec2i& indices)");
        auto& size = std::get<Vec2u>(frame);
        return Recti(size.x * indices.x, size.y * indices.y, size.x, size.y);
    }

    Texture::Texture(const io::File::Texture& texture_io) : texture_io(texture_io)
    {
    }

    bool Texture::Create(const Vec2u& size)
    {
        return texture_io.Create(size);
    }
    
    bool Texture::FromFile(const std::string& filename, const Recti& area)
    {
        return texture_io.FromFile(filename, area);
    }

    bool Texture::FromMemory(void* data, std::size_t size, const Recti& area)
    {
        return texture_io.FromMemory(data, size, area);
    }

    bool Texture::FromImage(const Image& image, const Recti& area)
    {
        return texture_io.FromImage(image.image_io, area);
    }
    
    void Texture::SetRenderOptions(RenderOptions options)
    {
        texture_io.SetSmooth(options & RenderOptions::Smooth);
        texture_io.SetSRGB(options & RenderOptions::sRGB);
        texture_io.SetRepeated(options & RenderOptions::Repeated);
    }

    bool Texture::IsSmooth() const
    {
        return texture_io.IsSmooth();
    }

    bool Texture::IsSRGB() const
    {
        return texture_io.IsSRGB();
    }

    bool Texture::IsRepeated() const
    {
        return texture_io.IsRepeated();
    }

    Vec2u Texture::GetSize() const
    {
        return texture_io.GetSize();
    }
}