#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>

namespace doge
{
    struct PolygonShape
    {
        struct Vertex
        {
            Vec2f position = Vec2f::Zero;
            Color color = Color::White;
            Vec2f texture_coordinates = Vec2f::Zero;
        };

        enum Type
        {
            Points,
            Lines,
            LineStrip,
            Triangles,
            TriangleStrip,
            TriangleFan,
            Quads,
        };

        Type type = Type::Points;
        std::vector<Vertex> vertices = std::vector<Vertex>();
        Vec2f origin = Vec2f::Zero;
        std::string texture_id = "";
    };
}