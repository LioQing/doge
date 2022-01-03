#pragma once

namespace doge
{
    class Rotation
    {
        float rotation = 0.f; // in radian

        bool is_modified = true;

    public:

        Rotation(const Rotation&) = default;
        Rotation(float rotation = 0.f);

        float& Get();
        float Get() const;

        float& _NoModify_Get();
        void _ResetModify();
        bool _IsModified() const;
    };
}