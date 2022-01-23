#pragma once

#include <set>
#include <concepts>

namespace doge
{
    struct Layer
    {
        std::set<std::int32_t> layers = { 0 };

        static Layer Create(std::int32_t layer)
        {
            return Layer(std::set{ layer });
        }

        template <std::integral... Layers>
        static Layer Create(Layers&&... layers)
        {
            return Layer(std::set{ static_cast<std::int32_t>(layers)... });
        }
    };
}