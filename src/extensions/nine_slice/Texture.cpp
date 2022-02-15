#include <doge/extensions/nine_slice/Texture.hpp>

#include <stdexcept>

namespace doge::nine_slice
{
    std::string Texture::PostFixFromSlice(Slice slice)
    {
        switch (slice)
        {
            case Slice::Center:        return "center";
            case Slice::TopLeft:       return "top_left";
            case Slice::Top:           return "top";
            case Slice::TopRight:      return "top_right";
            case Slice::Right:         return "right";
            case Slice::BottomRight:   return "bottom_right";
            case Slice::Bottom:        return "bottom";
            case Slice::BottomLeft:    return "bottom_left";
            case Slice::Left:          return "left";
            default: throw std::invalid_argument("Invalid Slice");
        }
    }

    Slice Texture::SliceFromPostFix(const std::string& post_fix)
    {
        if (post_fix == "center")          return Slice::Center;
        if (post_fix == "top_left")        return Slice::TopLeft;
        if (post_fix == "top")             return Slice::Top;
        if (post_fix == "top_right")       return Slice::TopRight;
        if (post_fix == "right")           return Slice::Right;
        if (post_fix == "bottom_right")    return Slice::BottomRight;
        if (post_fix == "bottom")          return Slice::Bottom;
        if (post_fix == "bottom_left")     return Slice::BottomLeft;
        if (post_fix == "left")            return Slice::Left;
        
        throw std::invalid_argument("Invalid post fix");
    }
}