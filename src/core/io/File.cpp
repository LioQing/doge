#include <doge/core/io/File.hpp>

#include <doge/utils.hpp>

namespace doge::io
{
    Vec2u File::Texture::GetSize() const
    {
        return cast::FromSfVec2(texture.getSize());
    }

    bool File::Texture::Create(const Vec2u& size)
    {
        return texture.create(size.x, size.y);
    }

    bool File::Texture::FromFile(const std::string& filename, const Recti& area)
    {
        return texture.loadFromFile(filename, cast::ToSfRect(area));
    }

    bool File::Texture::FromMemory(void* data, std::size_t size, const Recti& area)
    {
        return texture.loadFromMemory(data, size, cast::ToSfRect(area));
    }

    bool File::Texture::FromImage(const Image& image, const Recti& area)
    {
        return texture.loadFromImage(image.image, cast::ToSfRect(area));
    }

    void File::Texture::SetSmooth(bool smooth)
    {
        texture.setSmooth(smooth);
    }

    bool File::Texture::IsSmooth() const
    {
        return texture.isSmooth();
    }

    void File::Texture::SetSRGB(bool srgb)
    {
        texture.setSrgb(srgb);
    }

    bool File::Texture::IsSRGB() const
    {
        return texture.isSrgb();
    }

    void File::Texture::SetRepeated(bool repeated)
    {
        texture.setRepeated(repeated);
    }

    bool File::Texture::IsRepeated() const
    {
        return texture.isRepeated();
    }

    Vec2u File::Image::GetSize() const
    {
        return cast::FromSfVec2(image.getSize());
    }

    void File::Image::Create(const Vec2u& size, const Color& color)
    {
        image.create(size.x, size.y, cast::ToSfColor(color));
    }

    bool File::Image::FromFile(const std::string& filename)
    {
        return image.loadFromFile(filename);
    }

    bool File::Image::FromMemory(void* data, std::size_t size)
    {
        return image.loadFromMemory(data, size);
    }

    void File::Image::FromTexture(const Texture& texture)
    {
        image = texture.texture.copyToImage();
    }

    bool File::Image::ToFile(const std::string& filename) const 
    {
        return image.saveToFile(filename);
    }

    void File::Image::MaskColor(const Color& color, std::uint8_t alpha)
    {
        image.createMaskFromColor(cast::ToSfColor(color), alpha);
    }

    void File::Image::SetPixel(std::uint32_t x, std::uint32_t y, const Color& color)
    {
        image.setPixel(x, y, cast::ToSfColor(color));
    }

    Color File::Image::GetPixel(std::uint32_t x, std::uint32_t y) const
    {
        return cast::FromSfColor(image.getPixel(x, y));
    }

    const std::uint8_t* File::Image::GetPixelPtr() const
    {
        return image.getPixelsPtr();
    }
}