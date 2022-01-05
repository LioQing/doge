#pragma once

#include <doge/core/IOBus.hpp>
#include <doge/utils/Rect.hpp>
#include <vector>
#include <string>

namespace doge
{
    class Texture
    {
        std::string id;
        IOBus::Texture texture;
        std::unique_ptr<std::vector<Recti>> sprite_sheets_info;

    public:

        const std::string& GetID() const;
    };

    std::strong_ordering operator<=>(const Texture& t1, const Texture& t2);
}