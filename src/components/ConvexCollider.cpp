#include <doge/components/ConvexCollider.hpp>

namespace doge
{
    ConvexCollider::ConvexCollider(
            const std::vector<Vec2f>& points,
            const Vec2f& origin,
            float density,
            float restitution,
            float friction
        ) :
        points(points),
        origin(origin),
        density(density),
        restitution(restitution),
        friction(friction)
    {
    }

    std::vector<Vec2f>& ConvexCollider::GetPoints()
    {
        is_modified = true;
        return points;
    }

    const std::vector<Vec2f>& ConvexCollider::GetPoints() const
    {
        return points;
    }

    Vec2f& ConvexCollider::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& ConvexCollider::GetOrigin() const
    {
        return origin;    
    }

    float& ConvexCollider::GetDensity()
    {
        is_modified = true;
        return density;
    }

    float ConvexCollider::GetDensity() const
    {
        return density;    
    }

    float& ConvexCollider::GetRestitution()
    {
        is_modified = true;
        return restitution;
    }

    float ConvexCollider::GetRestitution() const
    {
        return restitution;    
    }

    float& ConvexCollider::GetFriction()
    {
        is_modified = true;
        return friction;
    }

    float ConvexCollider::GetFriction() const
    {
        return friction;    
    }

    std::vector<Vec2f>& ConvexCollider::_NoModify_GetPoints()
    {
        return points;
    }

    Vec2f& ConvexCollider::_NoModify_GetOrigin()
    {
        return origin;
    }

    float& ConvexCollider::_NoModify_GetDensity()
    {
        return density;
    }

    float& ConvexCollider::_NoModify_GetRestitution()
    {
        return restitution;
    }

    float& ConvexCollider::_NoModify_GetFriction()
    {
        return friction;
    }

    void ConvexCollider::_ResetModify()
    {
        is_modified = false;
    }

    bool ConvexCollider::_IsModified() const
    {
        return is_modified;
    }
}