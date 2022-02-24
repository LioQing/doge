#include <doge/components/CustomShape.hpp>

#include <algorithm>
#include <cmath>
#include <doge/utils/math.hpp>
#include <doge/utils/Rect.hpp>

namespace doge
{
    template <typename VertexType>
    static std::vector<VertexType> _TriToShapeVertices(
        const std::vector<VertexType>& src,
        const std::vector<std::array<std::size_t, 3>>& tri_indices
    )
    {
        std::vector<VertexType> shape_vert(tri_indices.size() * 3);
        for (std::size_t i = 0; i < tri_indices.size(); ++i)
        {
            shape_vert.at(i * 3 + 0) = src.at(tri_indices.at(i).at(0));
            shape_vert.at(i * 3 + 1) = src.at(tri_indices.at(i).at(1));
            shape_vert.at(i * 3 + 2) = src.at(tri_indices.at(i).at(2));
        }
        return shape_vert;
    }

    static std::vector<Vec2f> _MapTextureRectToCoords(const std::vector<Vec2f>& vertices, const std::vector<Vec2f>& to_be_mapped, const Rectf& texture_rectangle)
    {
        auto rect = Rectf(vertices.front(), vertices.front());
        for (auto& v : vertices)
        {
            rect.left   = std::min(rect.left, v.x);
            rect.top    = std::min(rect.top, v.y);
            rect.width  = std::max(rect.width, v.x);
            rect.height = std::max(rect.height, v.y);
        }
        rect.width -= rect.left;
        rect.height -= rect.top;

        std::vector<Vec2f> texture_coords(to_be_mapped.size());
        for (auto i = 0; i < to_be_mapped.size(); ++i)
        {
            texture_coords.at(i) = texture_rectangle.GetPosition() + (to_be_mapped.at(i) - rect.GetPosition()) / rect.GetSize() * texture_rectangle.GetSize();
        }

        return texture_coords;
    }

    static std::vector<Vec2f> _MapTextureRectToCoords(const std::vector<Vec2f>& vertices, const Rectf& texture_rectangle)
    {
        return _MapTextureRectToCoords(vertices, vertices, texture_rectangle);
    }

    CustomShape CustomShape::Create(
        Type type,
        const std::vector<Vertex> vertices,
        const Vec2f& origin,
        const std::string& texture_id
    )
    {
        return CustomShape(type, vertices, origin, texture_id);
    }

    CustomShape CustomShape::Create(
        Type type,
        const std::vector<Vec2f>& vertices,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle
    )
    {
        CustomShape shape
        {
            .type = type,
            .vertices = std::vector<Vertex>(vertices.size()),
            .origin = origin,
            .texture_id = texture_id,
        };

        auto texture_coords = _MapTextureRectToCoords(vertices, texture_rectangle);

        for (auto i = 0; i < vertices.size(); ++i)
        {
            shape.vertices.at(i).position = vertices.at(i);
            shape.vertices.at(i).color = color;
            shape.vertices.at(i).texture_coordinates = texture_coords.at(i);
        }
        
        return shape;
    }

    CustomShape CustomShape::CreatePolygon(
        const std::vector<Vertex>& vertices,
        const Vec2f& origin,
        const std::string& texture_id,
        polygon::TriType tri
    )
    {
        std::vector<Vec2f> v_vertices(vertices.size());
        std::transform(vertices.begin(), vertices.end(), v_vertices.begin(),
        [](const Vertex& v){ return v.position; });

        auto output = polygon::Triangulate(v_vertices, tri);

        return CustomShape
        {
            .type = Type::Triangles,
            .vertices = _TriToShapeVertices(vertices, output),
            .origin = origin,
            .texture_id = texture_id,
        };
    }

    CustomShape CustomShape::CreatePolygon(
        const std::vector<Vec2f>& vertices,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle,
        polygon::TriType tri
    )
    {
        auto output = polygon::Triangulate(vertices, tri);
        return Create(Type::Triangles, _TriToShapeVertices(vertices, output), color, origin, texture_id, texture_rectangle);
    }

    CustomShape CustomShape::CreateOutline(
        const std::vector<Vec2f>& vertices,
        float thickness,
        const Color& color,
        const Color& inner_color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle
    )
    {
        auto outline_verts = polygon::Outline(vertices, thickness);
        auto texture_coords = _MapTextureRectToCoords(vertices, outline_verts, texture_rectangle);

        std::vector<Vertex> verts(outline_verts.size());
        for (std::size_t i = 0; i < outline_verts.size(); i += 6)
        {
            verts.at(i + 0) = Vertex{ .position = outline_verts.at(i + 0), .color = inner_color, .texture_coordinates = texture_coords.at(i + 0) };
            verts.at(i + 1) = Vertex{ .position = outline_verts.at(i + 1), .color = color      , .texture_coordinates = texture_coords.at(i + 1) };
            verts.at(i + 2) = Vertex{ .position = outline_verts.at(i + 2), .color = inner_color, .texture_coordinates = texture_coords.at(i + 2) };

            verts.at(i + 3) = Vertex{ .position = outline_verts.at(i + 3), .color = inner_color, .texture_coordinates = texture_coords.at(i + 3) };
            verts.at(i + 4) = Vertex{ .position = outline_verts.at(i + 4), .color = color      , .texture_coordinates = texture_coords.at(i + 4) };
            verts.at(i + 5) = Vertex{ .position = outline_verts.at(i + 5), .color = color      , .texture_coordinates = texture_coords.at(i + 5) };
        }

        return CustomShape
        {
            .type = Type::Triangles,
            .vertices = verts,
            .origin = origin,
            .texture_id = texture_id,
        };
    }

    CustomShape CustomShape::CreateOutline(
        const std::vector<Vec2f>& vertices,
        float thickness,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle
    )
    {
        return CreateOutline(vertices, thickness, color, color, origin, texture_id, texture_rectangle);
    }

    CustomShape CustomShape::CreateCircle(
        float radius,
        std::size_t vertex_count,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle
    )
    {
        auto verts = polygon::Circle(radius, vertex_count);
        auto texture_coords = _MapTextureRectToCoords(verts, texture_rectangle);

        std::vector<Vertex> vertices(verts.size());
        for (std::size_t i = 0; i < vertices.size(); ++i)
        {
            vertices.at(i) = Vertex{ .position = verts.at(i), .color = color, .texture_coordinates = texture_coords.at(i) };
        }

        return CustomShape
        {
            .type = Type::TriangleFan,
            .vertices = vertices,
            .origin = origin,
            .texture_id = texture_id,
        };
    }

    CustomShape CustomShape::CreateRectangle(
        const Vec2f& size,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle
    )
    {
        auto corner = size / 2.f;
        auto verts = std::vector<Vec2f>{ -corner, corner * Vec2f(1, -1), corner * Vec2f(-1, 1), corner };
        auto texture_coords = std::vector<Vec2f>
        {
            texture_rectangle.GetPosition(),
            texture_rectangle.GetPosition() + Vec2f(texture_rectangle.width, 0),
            texture_rectangle.GetPosition() + Vec2f(0, texture_rectangle.height),
            texture_rectangle.GetPosition() + texture_rectangle.GetSize(),
        };

        std::vector<Vertex> vertices(verts.size());
        for (std::size_t i = 0; i < vertices.size(); ++i)
        {
            vertices.at(i) = Vertex{ .position = verts.at(i), .color = color, .texture_coordinates = texture_coords.at(i) };
        }

        return CustomShape
        {
            .type = Type::TriangleStrip,
            .vertices = vertices,
            .origin = origin,
            .texture_id = texture_id,
        };
    }

    CustomShape CustomShape::CreateConvex(
        const std::vector<Vertex>& vertices,
        const Vec2f& origin,
        const std::string& texture_id
    )
    {
        return CustomShape
        {
            .type = Type::TriangleFan,
            .vertices = vertices,
            .origin = origin,
            .texture_id = texture_id,
        };
    }

    CustomShape CustomShape::CreateConvex(
        const std::vector<Vec2f>& vertices,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Rectf& texture_rectangle
    )
    {
        auto texture_coords = _MapTextureRectToCoords(vertices, texture_rectangle);

        std::vector<Vertex> verts(vertices.size());
        for (std::size_t i = 0; i < verts.size(); ++i)
        {
            verts.at(i) = Vertex{ .position = vertices.at(i), .color = color, .texture_coordinates = texture_coords.at(i) };
        }

        return CustomShape
        {
            .type = Type::TriangleFan,
            .vertices = verts,
            .origin = origin,
            .texture_id = texture_id,
        };
    }

    void CustomShape::SetColor(const Color& color)
    {
        for (auto& v : vertices)
        {
            v.color = color;
        }
    }

    void CustomShape::SetTextureRectangle(const Rectf& rectangle)
    {
        std::vector<Vec2f> verts(vertices.size());
        std::transform(vertices.begin(), vertices.end(), verts.end(),
        [](const Vertex& v){ return v.position; });

        auto texture_coords = _MapTextureRectToCoords(verts, rectangle);
        for (std::size_t i = 0; i < vertices.size(); ++i)
        {
            vertices.at(i).texture_coordinates = texture_coords.at(i);
        }
    }

    void CustomShape::SetTextureRectangle(const Rectf& rectangle, const std::vector<Vec2f>& relative_to)
    {
        std::vector<Vec2f> verts(vertices.size());
        std::transform(vertices.begin(), vertices.end(), verts.end(),
        [](const Vertex& v){ return v.position; });

        auto texture_coords = _MapTextureRectToCoords(verts, relative_to, rectangle);
        for (std::size_t i = 0; i < vertices.size(); ++i)
        {
            vertices.at(i).texture_coordinates = texture_coords.at(i);
        }
    }
}