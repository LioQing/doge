#include <doge/components/Position.hpp>

namespace doge
{
    Position::Position(const Vec2f& position) : position(position) 
    {
    }

    Position::Position(float x, float y) : position(x, y) 
    {
    }

    Vec2f& Position::Get()
    {
        is_modified = true;
        return position;
    }

    const Vec2f& Position::Get() const
    {
        return position;
    }

    Vec2f& Position::_NoModify_Get()
    {
        return position;
    }

    void Position::_ResetModify()
    {
        is_modified = false;
    }

    bool Position::_IsModified() const
    {
        return is_modified;
    }
}