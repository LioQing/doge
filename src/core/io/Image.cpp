#include <doge/core/io/Image.hpp>

#include <doge/utils.hpp>
#include <doge/core.hpp>

namespace doge::io
{
    Vec2u Image::GetSize() const
    {
        return cast::FromSfVec2(image.getSize());
    }

    void Image::Create(const Vec2u& size, const Color& color)
    {
        image.create(size.x, size.y, cast::ToSfColor(color));
    }

    bool Image::FromFile(const std::string& filename)
    {
        return image.loadFromFile(filename);
    }

    bool Image::FromMemory(void* data, std::size_t size)
    {
        return image.loadFromMemory(data, size);
    }

    void Image::DownloadFromTexture(const Texture& texture)
    {
        image = texture.texture.copyToImage();
    }

    bool Image::ToFile(const std::string& filename) const 
    {
        return image.saveToFile(filename);
    }

    void Image::MaskColor(const Color& color, std::uint8_t alpha)
    {
        image.createMaskFromColor(cast::ToSfColor(color), alpha);
    }

    void Image::SetPixel(std::uint32_t x, std::uint32_t y, const Color& color)
    {
        image.setPixel(x, y, cast::ToSfColor(color));
    }

    Color Image::GetPixel(std::uint32_t x, std::uint32_t y) const
    {
        return cast::FromSfColor(image.getPixel(x, y));
    }

    const std::uint8_t* Image::GetPixelPtr() const
    {
        return image.getPixelsPtr();
    }
}