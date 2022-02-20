#include <doge/components/CustomShape.hpp>

#include <algorithm>
#include <doge/utils/math.hpp>
#include <doge/utils/Rect.hpp>

namespace doge
{
    CustomShape CustomShape::Create(
        Type type,
        const std::vector<Vec2f>& vertices,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Recti& texture_rectangle
    )
    {
        CustomShape shape
        {
            .type = type,
            .vertices = std::vector<Vertex>(vertices.size()),
            .origin = origin,
            .texture_id = texture_id,
        };

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

        for (auto i = 0; i < vertices.size(); ++i)
        {
            shape.vertices.at(i).position = vertices.at(i);
            shape.vertices.at(i).color = color;
            shape.vertices.at(i).texture_coordinates = texture_rectangle.GetPosition() + (vertices.at(i) - rect.GetPosition()) / rect.GetSize() * texture_rectangle.GetSize();
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

        CustomShape shape
        {
            .type = Type::Triangles,
            .vertices = std::vector<Vertex>(output.size() * 3),
            .origin = origin,
            .texture_id = texture_id,
        };

        for (auto i = 0; i < output.size(); ++i)
        {
            shape.vertices.at(i * 3 + 0) = vertices.at(output.at(i).at(0));
            shape.vertices.at(i * 3 + 1) = vertices.at(output.at(i).at(1));
            shape.vertices.at(i * 3 + 2) = vertices.at(output.at(i).at(2));
        }

        return shape;
    }

    CustomShape CustomShape::CreatePolygon(
        const std::vector<Vec2f>& vertices,
        const Color& color,
        const Vec2f& origin,
        const std::string& texture_id,
        const Recti& texture_rectangle,
        triangulation::Type tri
    )
    {
        auto output = triangulation::Triangulate(vertices, tri);

        std::vector<Vec2f> shape_vertices(output.size() * 3);
        for (auto i = 0; i < output.size(); ++i)
        {
            shape_vertices.at(i * 3 + 0) = vertices.at(output.at(i).at(0));
            shape_vertices.at(i * 3 + 1) = vertices.at(output.at(i).at(1));
            shape_vertices.at(i * 3 + 2) = vertices.at(output.at(i).at(2));
        }

        return Create(Type::Triangles, shape_vertices, color, origin, texture_id, texture_rectangle);
    }
}