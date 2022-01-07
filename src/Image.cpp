#include <doge/core/Image.hpp>

#include <doge/core/Texture.hpp>
#include <doge/utils.hpp>

namespace doge
{
    Image::Image(const IOBus::ImageData& image_data) : image_data(image_data)
    {
    }

    Image::Image(const Vec2u& size, const Color& color)
    {
        image_data.Create(size, color);
    }
    
    Image::Image(const std::string& filename)
    {
        image_data.FromFile(filename);
    }

    Image::Image(void* data, std::size_t size)
    {
        image_data.FromMemory(data, size);
    }

    Image::Image(const Texture& texture)
    {
        image_data.FromTexture(texture.texture_data);
    }

    Vec2u Image::GetSize() const
    {
        return image_data.GetSize();
    }

    void Image::ToFile(const std::string& filename) const 
    {
        image_data.ToFile(filename);
    }

    void Image::MaskColor(const Color& color, std::uint8_t alpha)
    {
        image_data.MaskColor(color, alpha);
    }

    void Image::SetPixel(std::uint32_t x, std::uint32_t y, const Color& color)
    {
        image_data.SetPixel(x, y, color);
    }

    Color Image::GetPixel(std::uint32_t x, std::uint32_t y) const
    {
        return image_data.GetPixel(x, y);
    }

    const std::uint8_t* Image::GetPixelPtr() const
    {
        return image_data.GetPixelPtr();
    }
}