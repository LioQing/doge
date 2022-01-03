#pragma once

#include <doge/utils/Vec2.hpp>

namespace doge
{
    class CircleCollider
    {
        float radius = 1.f;
        Vec2f origin = Vec2f::Zero();
        
        float density = 0.f;
        float restitution = 0.f;
        float friction = 0.2f;

        bool is_modified = true;

    public:

        CircleCollider(const CircleCollider&) = default;
        CircleCollider(
            float radius = 1.f,
            const Vec2f& origin = Vec2f::Zero(),
            float density = 0.f,
            float restitution = 0.f,
            float friction = 0.2f
        );

        float& GetRadius();
        float GetRadius() const;

        Vec2f& GetOrigin();
        const Vec2f& GetOrigin() const;
        
        float& GetDensity();
        float GetDensity() const;
        
        float& GetRestitution();
        float GetRestitution() const;
        
        float& GetFriction();
        float GetFriction() const;

        float& _NoModify_GetRadius();
        Vec2f& _NoModify_GetOrigin();
        float& _NoModify_GetDensity();
        float& _NoModify_GetRestitution();
        float& _NoModify_GetFriction();
        void _ResetModify();
        bool _IsModified() const;
    };
}