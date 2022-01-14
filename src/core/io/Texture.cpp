#include <doge/core/io/Texture.hpp>

#include <doge/utils.hpp>
#include <doge/core.hpp>

namespace doge::io
{
    Vec2u Texture::GetSize() const
    {
        return cast::FromSfVec2(texture.getSize());
    }

    bool Texture::Create(const Vec2u& size)
    {
        return texture.create(size.x, size.y);
    }

    bool Texture::FromFile(const std::string& filename, const Recti& area)
    {
        return texture.loadFromFile(filename, cast::ToSfRect(area));
    }

    bool Texture::FromMemory(void* data, std::size_t size, const Recti& area)
    {
        return texture.loadFromMemory(data, size, cast::ToSfRect(area));
    }

    bool Texture::FromImage(const Image& image, const Recti& area)
    {
        return texture.loadFromImage(image.image, cast::ToSfRect(area));
    }

    void Texture::SetSmooth(bool smooth)
    {
        texture.setSmooth(smooth);
    }

    bool Texture::IsSmooth() const
    {
        return texture.isSmooth();
    }

    void Texture::SetSRGB(bool srgb)
    {
        texture.setSrgb(srgb);
    }

    bool Texture::IsSRGB() const
    {
        return texture.isSrgb();
    }

    void Texture::SetRepeated(bool repeated)
    {
        texture.setRepeated(repeated);
    }

    bool Texture::IsRepeated() const
    {
        return texture.isRepeated();
    }
}