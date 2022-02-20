#include <doge/utils/triangulation.hpp>

#include <doge/utils/polypartition/polypartition.h>

namespace doge
{
    std::vector<std::array<std::size_t, 3>> triangulation::Triangulate(const std::vector<Vec2f>& vertices, Type type)
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
        if (type == Type::Fast)
        {
            pp.Triangulate_MONO(&poly, &output);
        }
        else if (type == Type::Default)
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
}