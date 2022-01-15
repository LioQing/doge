#include <doge/core/io/Cursor.hpp>

#include <doge/core/io/Image.hpp>
#include <doge/utils/cast.hpp>

namespace doge::io
{
    bool Cursor::FromFile(const std::string& filename, const Vec2u& hotspot)
    {
        Image image;
        
        if (!image.FromFile(filename))
            return false;
        
        return cursor.loadFromPixels(image.GetPixelPtr(), cast::ToSfVec2(image.GetSize()), cast::ToSfVec2(hotspot));
    }

    bool Cursor::FromImage(const Image& image, const Vec2u& hotspot)
    {
        return cursor.loadFromPixels(image.GetPixelPtr(), cast::ToSfVec2(image.GetSize()), cast::ToSfVec2(hotspot));
    }

    bool Cursor::FromPixels(const std::uint8_t* pixels, const Vec2u& size, const Vec2u& hotspot)
    {
        return cursor.loadFromPixels(pixels, cast::ToSfVec2(size), cast::ToSfVec2(hotspot));
    }

    bool Cursor::FromSystem(Type type)
    {
        return cursor.loadFromSystem(type);
    }
}