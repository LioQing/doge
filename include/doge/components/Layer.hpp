#pragma once

#include <set>
#include <concepts>

namespace doge
{
    struct Layer
    {
        std::set<std::int32_t> layers = { 0 };

        std::int32_t GetFirst() const
        {
            return layers.begin().operator*();
        }

        static Layer CreateRange(std::int32_t start, std::int32_t end)
        {
            Layer layer;
            layer.layers.clear();
            for (std::int32_t i = start; i < end; ++i)
            {
                layer.layers.emplace(i);
            }
            return layer;
        }

        template <std::integral... Layers>
        static Layer Create(Layers&&... layers)
        {
            return Layer(std::set{ static_cast<std::int32_t>(layers)... });
        }
    };
}