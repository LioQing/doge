#pragma once

#include <doge/utils/aliases.hpp>
#include <doge/core/Image.hpp>
#include <unordered_map>
#include <string>

namespace doge
{
    struct Assets
    {
        std::unordered_map<std::string, Image> images;
    };
}