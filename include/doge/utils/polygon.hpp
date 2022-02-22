#pragma once

#include <vector>
#include <array>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    struct polygon
    {
        polygon(const polygon&) = delete;

        enum TriType
        {
            Fast = 0,
            Default = 1,
            Optimal = 2,

            EarClipping = Default,
            Monotone = Fast,
        };

        static std::vector<std::array<std::size_t, 3>> Triangulate(const std::vector<Vec2f>& vertices, TriType type = TriType::Default);

        // return: { 0in, 1out, 1in (0), 0in, 0out, 1out (0), 1in, 2out, 2in (1), 1in, 1out, 2out (1), ... }
        static std::vector<Vec2f> Outline(const std::vector<Vec2f>& vertices, float thickness);

        static std::vector<Vec2f> RoundedRectangle(const Vec2f& size, float radius, std::uint32_t corner_vertex_count = 8);
    };
}