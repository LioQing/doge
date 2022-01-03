#include <doge/components/RectangleCollider.hpp>

namespace doge
{
    RectangleCollider::RectangleCollider(
            Vec2f size,
            Vec2f origin,
            float density,
            float restitution,
            float friction
        ) :
        size(size),
        origin(origin),
        density(density),
        restitution(restitution),
        friction(friction)
    {
    }

    Vec2f& RectangleCollider::GetSize()
    {
        is_modified = true;
        return size;
    }

    const Vec2f& RectangleCollider::GetSize() const
    {
        return size;
    }

    Vec2f& RectangleCollider::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& RectangleCollider::GetOrigin() const
    {
        return origin;
    }

    float& RectangleCollider::GetDensity()
    {
        is_modified = true;
        return density;
    }

    float RectangleCollider::GetDensity() const
    {
        return density;
    }

    float& RectangleCollider::GetRestitution()
    {
        is_modified = true;
        return restitution;
    }

    float RectangleCollider::GetRestitution() const
    {
        return restitution;
    }

    float& RectangleCollider::GetFriction()
    {
        is_modified = true;
        return friction;
    }

    float RectangleCollider::GetFriction() const
    {
        return friction;
    }

    Vec2f& RectangleCollider::_NoModify_GetSize()
    {
        return size;
    }

    Vec2f& RectangleCollider::_NoModify_GetOrigin()
    {
        return origin;
    }

    float& RectangleCollider::_NoModify_GetDensity()
    {
        return density;
    }

    float& RectangleCollider::_NoModify_GetRestitution()
    {
        return restitution;
    }

    float& RectangleCollider::_NoModify_GetFriction()
    {
        return friction;
    }

    void RectangleCollider::_ResetModify()
    {
        is_modified = false;
    }

    bool RectangleCollider::_IsModified() const
    {
        return is_modified;
    }
}