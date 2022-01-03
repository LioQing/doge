#include <doge/components/Rotation.hpp>

namespace doge
{
    Rotation::Rotation(float rotation) : rotation(rotation) 
    {
    }

    float& Rotation::Get()
    {
        is_modified = false;
        return rotation;
    }

    float Rotation::Get() const
    {
        return rotation;
    }

    float& Rotation::_NoModify_Get()
    {
        return rotation;
    }

    void Rotation::_ResetModify()
    {
        is_modified = false;
    }

    bool Rotation::_IsModified() const
    {
        return is_modified;
    }
}