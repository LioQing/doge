#include <doge/core/Image.hpp>

#include <doge/core/Texture.hpp>
#include <doge/utils.hpp>

namespace doge
{
    Image::Image(const io::File::Image& image_io) : image_io(image_io)
    {
    }

    void Image::Create(const Vec2u& size, const Color& color)
    {
        image_io.Create(size, color);
    }
    
    bool Image::FromFile(const std::string& filename)
    {
        return image_io.FromFile(filename);
    }

    bool Image::FromMemory(void* data, std::size_t size)
    {
        return image_io.FromMemory(data, size);
    }

    void Image::FromTexture(const Texture& texture)
    {
        image_io.FromTexture(texture.texture_io);
    }

    Vec2u Image::GetSize() const
    {
        return image_io.GetSize();
    }

    void Image::ToFile(const std::string& filename) const 
    {
        image_io.ToFile(filename);
    }

    void Image::MaskColor(const Color& color, std::uint8_t alpha)
    {
        image_io.MaskColor(color, alpha);
    }

    void Image::SetPixel(std::uint32_t x, std::uint32_t y, const Color& color)
    {
        image_io.SetPixel(x, y, color);
    }

    Color Image::GetPixel(std::uint32_t x, std::uint32_t y) const
    {
        return image_io.GetPixel(x, y);
    }

    const std::uint8_t* Image::GetPixelPtr() const
    {
        return image_io.GetPixelPtr();
    }
}