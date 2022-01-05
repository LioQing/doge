#include <doge/core/Texture.hpp>

namespace doge
{
    const std::string& Texture::GetID() const
    {
        return id;
    }

    std::strong_ordering operator<=>(const Texture& t1, const Texture& t2)
    {
        return t1.GetID() <=> t2.GetID();
    }
}