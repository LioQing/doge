#include <doge/core/Image.hpp>

#include <doge/core/Texture.hpp>

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
}