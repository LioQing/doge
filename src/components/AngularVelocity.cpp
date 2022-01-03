#include <doge/components/AngularVelocity.hpp>

namespace doge
{
    AngularVelocity::AngularVelocity(float angular_velocity) : angular_velocity(angular_velocity) 
    {
    }

    float& AngularVelocity::Get()
    {
        is_modified = false;
        return angular_velocity;
    }

    float AngularVelocity::Get() const
    {
        return angular_velocity;
    }

    float& AngularVelocity::_NoModify_Get()
    {
        return angular_velocity;
    }

    void AngularVelocity::_ResetModify()
    {
        is_modified = false;
    }

    bool AngularVelocity::_IsModified() const
    {
        return is_modified;
    }
}