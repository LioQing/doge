#include <doge/components/CircleCollider.hpp>

namespace doge
{
    CircleCollider::CircleCollider(
            float radius,
            const Vec2f& origin,
            float density,
            float restitution,
            float friction
        ) : 
        radius(radius), 
        origin(origin), 
        density(density), 
        restitution(restitution),
        friction(friction)
    {
    }

    float& CircleCollider::GetRadius()
    {
        is_modified = true;
        return radius;
    }

    float CircleCollider::GetRadius() const
    {
        return radius;    
    }

    Vec2f& CircleCollider::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& CircleCollider::GetOrigin() const
    {
        return origin;    
    }

    float& CircleCollider::GetDensity()
    {
        is_modified = true;
        return density;
    }

    float CircleCollider::GetDensity() const
    {
        return density;    
    }

    float& CircleCollider::GetRestitution()
    {
        is_modified = true;
        return restitution;
    }

    float CircleCollider::GetRestitution() const
    {
        return restitution;    
    }

    float& CircleCollider::GetFriction()
    {
        is_modified = true;
        return friction;
    }

    float CircleCollider::GetFriction() const
    {
        return friction;    
    }

    float& CircleCollider::_NoModify_GetRadius()
    {
        return radius;    
    }

    Vec2f& CircleCollider::_NoModify_GetOrigin()
    {
        return origin;    
    }

    float& CircleCollider::_NoModify_GetDensity()
    {
        return density;    
    }

    float& CircleCollider::_NoModify_GetRestitution()
    {
        return restitution;    
    }

    float& CircleCollider::_NoModify_GetFriction()
    {
        return friction;    
    }

    void CircleCollider::_ResetModify()
    {
        is_modified = false;
    }

    bool CircleCollider::_IsModified() const
    {
        return is_modified;
    }
}