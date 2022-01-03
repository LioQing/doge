#include <doge/components/RigidBody.hpp>

namespace doge
{
    RigidBody::RigidBody(Type type) : type(type)
    {
    }

    RigidBody::Type& RigidBody::GetType()
    {
        is_modified = true;
        return type;
    }

    RigidBody::Type RigidBody::GetType() const
    {
        return type;
    }

    RigidBody::Type& RigidBody::_NoModify_GetType()
    {
        return type;
    }

    void RigidBody::_ResetModify()
    {
        is_modified = false;
    }

    bool RigidBody::_IsModified() const
    {
        return is_modified;
    }
}