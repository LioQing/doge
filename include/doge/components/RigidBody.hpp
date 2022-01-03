#pragma once

#include <box2d/box2d.h>

namespace doge
{
    class RigidBody
    {
    public:
        
        enum Type
        {
            Static,
            Kinematic,
            Dynamic,
        };

    private:

        Type type = Type::Static;

        bool is_modified = true;

    public:

        RigidBody(const RigidBody&) = default;
        RigidBody(Type type = Type::Static);

        Type& GetType();
        Type GetType() const;

        Type& _NoModify_GetType();
        void _ResetModify();
        bool _IsModified() const;
    };
}