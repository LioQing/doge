#include <doge/utils/polygon.hpp>

#include <doge/utils/polypartition/polypartition.h>
#include <numbers>

namespace doge
{
    std::vector<std::array<std::size_t, 3>> polygon::Triangulate(const std::vector<Vec2f>& vertices, TriType type)
    {
        tppl::TPPLPoly poly;
        poly.Init(vertices.size());
        for (std::size_t i = 0; i < vertices.size(); ++i)
        {
            poly[i].x = vertices.at(i).x;
            poly[i].y = vertices.at(i).y;
            poly[i].id = i;
        }

        tppl::TPPLPartition pp;
        tppl::TPPLPolyList output;
        if (type == TriType::Fast)
        {
            pp.Triangulate_MONO(&poly, &output);
        }
        else if (type == TriType::Default)
        {
            pp.Triangulate_EC(&poly, &output);
        }
        else
        {
            pp.Triangulate_OPT(&poly, &output);
        }
        
        std::vector<std::array<std::size_t, 3>> result(output.size());
        auto out_itr = output.begin();
        std::size_t i = 0;
        for (; i < result.size(); ++i, ++out_itr)
        {
            result.at(i).at(0) = (*out_itr)[0].id;
            result.at(i).at(1) = (*out_itr)[1].id;
            result.at(i).at(2) = (*out_itr)[2].id;
        }

        return result;
    }

    std::vector<Vec2f> polygon::Outline(const std::vector<Vec2f>& vertices, float thickness)
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

    std::vector<Vec2f> polygon::Circle(float radius, std::size_t vertex_count, const Vec2f& offset)
    {
        using std::numbers::pi;

        std::vector<Vec2f> vertices(vertex_count);

        for (std::size_t i = 0; i < vertex_count; ++i)
        {
            auto theta = 2 * pi / vertex_count * i;

            vertices.at(i) = (Vec2f::Right * radius).Rotated(theta); + offset;
        }

        return vertices;
    }

    std::vector<Vec2f> polygon::RoundedRectangle(const Vec2f& size, float radius, std::uint32_t corner_vertex_count, const Vec2f& offset)
    {
        using std::numbers::pi;

        if (corner_vertex_count == 1)
            return std::vector<Vec2f>{ size, Vec2f(size.x, 0), Vec2f::Zero, Vec2f(0, size.y) };

        std::vector<Vec2f> vertices(corner_vertex_count * 4);

        for (auto i = 0; i < corner_vertex_count; ++i)
        {
            auto theta = pi * (corner_vertex_count - i - 1) / 2.f / (corner_vertex_count - 1);

            vertices.at(i)                           = (Vec2f::Right * radius).Rotated(theta)                   + size - Vec2f(radius, radius)      + offset;
            vertices.at(corner_vertex_count * 1 + i) = (Vec2f::Right * radius).Rotated(theta - pi / 2.f)        + Vec2f(size.x - radius, radius)    + offset;
            vertices.at(corner_vertex_count * 2 + i) = (Vec2f::Right * radius).Rotated(theta - pi)              + Vec2f(radius, radius)             + offset;
            vertices.at(corner_vertex_count * 3 + i) = (Vec2f::Right * radius).Rotated(theta - pi * 3.f / 2.f)  + Vec2f(radius, size.y - radius)    + offset;
        }

        return vertices;
    }
}