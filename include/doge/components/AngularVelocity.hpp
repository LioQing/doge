#pragma once

namespace doge
{
    class AngularVelocity
    {
        float angular_velocity = 0.f; // in radian

        bool is_modified = true;

    public:

        AngularVelocity(const AngularVelocity&) = default;
        AngularVelocity(float angular_velocity = 0.f);

        float& Get();
        float Get() const;

        float& _NoModify_Get();
        void _ResetModify();
        bool _IsModified() const;
    };
}