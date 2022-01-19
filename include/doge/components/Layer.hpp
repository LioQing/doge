#pragma once

#include <set>
#include <concepts>

namespace doge
{
    struct Layer
    {
        std::set<int> layers = { 0 };

        static Layer Create(int layer)
        {
            return Layer(std::set{ layer });
        }

        template <std::integral... Layers>
        static Layer Create(Layers&&... layers)
        {
            return Layer(std::set{ static_cast<int>(layers)... });
        }
    };
}