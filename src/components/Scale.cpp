#include <doge/components/Scale.hpp>

namespace doge
{
    Scale::Scale(const Vec2f& scale) : scale(scale) 
    {
    }

    Scale::Scale(float x, float y) : scale(x, y) 
    {
    }

    Vec2f& Scale::Get()
    {
        is_modified = true;
        return scale;
    }

    const Vec2f& Scale::Get() const
    {
        return scale;
    }

    Vec2f& Scale::_NoModify_Get()
    {
        return scale;
    }

    void Scale::_ResetModify()
    {
        is_modified = false;
    }

    bool Scale::_IsModified() const
    {
        return is_modified;
    }
}