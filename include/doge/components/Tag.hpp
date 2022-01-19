#pragma once

#include <unordered_set>
#include <string>
#include <concepts>

namespace doge
{
    struct Tag
    {
        std::unordered_set<std::string> tags;

        static Tag Create(const std::string& tag)
        {
            return Tag(std::unordered_set{ tag });
        }

        template <std::convertible_to<std::string>... Tags>
        static Tag Create(Tags&&... tags)
        {
            return Tag(std::unordered_set{ static_cast<std::string>(tags)... });
        }
    };
}