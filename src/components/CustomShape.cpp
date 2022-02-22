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

    // at the returned vec: 0in, 1out, 1in (0), 0in, 0out, 1out (0), 1in, 2out, 2in (1), 1in, 1out, 2out (1), ...
    static std::vector<Vec2f> _GenOutlineVertices(const std::vector<Vec2f>& vertices, float thickness)
    {
        std::vector<Vec2f> outline_verts(vertices.size() * 6);

        auto GetOutterVertex = [&](std::size_t i)
        {
            auto last = vertices.at(i + (i == 0) * vertices.size() - 1);
            auto curr = vertices.at(i);
            auto next = vertices.at((i + 1) % vertices.size());

            auto a = last - curr;
            auto b = next - curr;
            auto c = (a * b.Magnitude() + b * a.Magnitude()).Normalized();

            auto cp = a.Cross(b);
            auto angle = std::acos(a.Dot(b) / (a.Magnitude() * b.Magnitude())) / 2.f;
            c *= cp / std::abs(cp) * thickness / std::sin(angle);

            return c + curr;
        };
        
        auto curr_out_vert = GetOutterVertex(0);
        for (std::size_t i = 0; i < vertices.size(); ++i)
        {
            auto curr_vert = vertices.at(i);
            auto next_vert = vertices.at((i + 1) % vertices.size());
            auto next_out_vert = GetOutterVertex((i + 1) % vertices.size());

            outline_verts.emplace_back(curr_vert);
            outline_verts.emplace_back(next_out_vert);
            outline_verts.emplace_back(next_vert);

            outline_verts.emplace_back(curr_vert);
            outline_verts.emplace_back(curr_out_vert);
            outline_verts.emplace_back(next_out_vert);

            curr_out_vert = next_out_vert;
        }

        return outline_verts;
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
        triangulation::Type tri
    )
    {
        std::vector<Vec2f> v_vertices(vertices.size());
        std::transform(vertices.begin(), vertices.end(), v_vertices.begin(),
        [](const Vertex& v){ return v.position; });

        auto output = triangulation::Triangulate(v_vertices, tri);

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
        triangulation::Type tri
    )
    {
        auto output = triangulation::Triangulate(vertices, tri);
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
        auto outline_verts = _GenOutlineVertices(vertices, thickness);
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
}