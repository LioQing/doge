#include <doge/components/Position.hpp>

namespace doge
{
    Position::Position(const Vec2f& position) : position(position) 
    {
    }

    Position::Position(float x, float y) : position(x, y) 
    {
    }
}