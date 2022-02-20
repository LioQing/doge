#pragma once

#include <vector>
#include <array>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct triangulation
    {
        triangulation(const triangulation&) = delete;

        enum Type
        {
            Fast = 0,
            Default = 1,
            Optimal = 2,

            EarClipping = Default,
            Monotone = Fast,
        };

        static std::vector<std::array<std::size_t, 3>> Triangulate(const std::vector<Vec2f>& vertices, Type type = Type::Default);
    };
}