#include <doge/components/Velocity.hpp>

namespace doge
{
    Velocity::Velocity(const Vec2f& velocity) : velocity(velocity) 
    {
    }

    Velocity::Velocity(float x, float y) : velocity(x, y) 
    {
    }

    Vec2f& Velocity::Get()
    {
        is_modified = true;
        return velocity;
    }

    const Vec2f& Velocity::Get() const
    {
        return velocity;
    }

    Vec2f& Velocity::_NoModify_Get()
    {
        return velocity;
    }

    void Velocity::_ResetModify()
    {
        is_modified = false;
    }

    bool Velocity::_IsModified() const
    {
        return is_modified;
    }
}