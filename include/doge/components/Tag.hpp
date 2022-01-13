#pragma once

#include <unordered_set>
#include <string>
#include <string>

namespace doge
{
    struct Tag
    {
        std::unordered_set<std::string> tags;

        static Tag Create(const std::string& tag)
        {
            return Tag(std::unordered_set{ tag });
        }

        template <std::convertible_to<std::string>... TTags>
        static Tag Create(TTags&&... tags)
        {
            return Tag(std::unordered_set{ std::string(tags)... });
        }
    };
}