#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/triangulation.hpp>
#include <vector>

namespace doge
{
    struct CustomShape
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

        static CustomShape Create(
            Type type,
            const std::vector<Vec2f>& vertices,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Recti& texture_rectangle = Recti()
        );

        static CustomShape CreatePolygon(
            const std::vector<Vertex>& vertices,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            triangulation::Type tri = triangulation::Type::Default
        );

        static CustomShape CreatePolygon(
            const std::vector<Vec2f>& vertices,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Recti& texture_rectangle = Recti(),
            triangulation::Type tri = triangulation::Type::Default
        );
    };
}