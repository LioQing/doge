#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Color.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/polygon.hpp>
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
            Type type = Type::Points,
            const std::vector<Vertex> vertices = std::vector<Vertex>(),
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = ""
        );

        static CustomShape Create(
            Type type,
            const std::vector<Vec2f>& vertices,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf()
        );

        static CustomShape CreatePolygon(
            const std::vector<Vertex>& vertices,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            polygon::TriType tri = polygon::TriType::Default
        );

        static CustomShape CreatePolygon(
            const std::vector<Vec2f>& vertices,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf(),
            polygon::TriType tri = polygon::TriType::Default
        );

        static CustomShape CreateOutline(
            const std::vector<Vec2f>& vertices,
            float thickness = 1.f,
            const Color& color = Color::White,
            const Color& inner_color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf()
        );

        static CustomShape CreateOutline(
            const std::vector<Vec2f>& vertices,
            float thickness = 1.f,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf()
        );

        static CustomShape CreateCircle(
            float radius,
            std::size_t vertex_count = 32,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf()
        );

        static CustomShape CreateRectangle(
            const Vec2f& size,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf()
        );

        static CustomShape CreateConvex(
            const std::vector<Vertex>& vertices,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = ""
        );

        static CustomShape CreateConvex(
            const std::vector<Vec2f>& vertices,
            const Color& color = Color::White,
            const Vec2f& origin = Vec2f::Zero,
            const std::string& texture_id = "",
            const Rectf& texture_rectangle = Rectf()
        );
    };
}