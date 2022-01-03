#include <doge/components/EdgeCollider.hpp>

namespace doge
{
    EdgeCollider::EdgeCollider(
            const std::vector<Vec2f>& points,
            bool is_loop,
            const Vec2f& origin,
            float density,
            float restitution,
            float friction
        ) :
        points(points),
        is_loop(is_loop),
        origin(origin),
        density(density),
        restitution(restitution),
        friction(friction)
    {
    }

    std::vector<Vec2f>& EdgeCollider::GetPoints()
    {
        is_modified = true;
        return points;
    }

    const std::vector<Vec2f>& EdgeCollider::GetPoints() const
    {
        return points;
    }

    bool& EdgeCollider::GetIsLoop()
    {
        is_modified = true;
        return is_loop;
    }

    bool EdgeCollider::GetIsLoop() const
    {
        return is_loop;
    }

    Vec2f& EdgeCollider::GetOrigin()
    {
        is_modified = true;
        return origin;
    }

    const Vec2f& EdgeCollider::GetOrigin() const
    {
        return origin;
    }

    float& EdgeCollider::GetDensity()
    {
        is_modified = true;
        return density;
    }

    float EdgeCollider::GetDensity() const
    {
        return density;
    }

    float& EdgeCollider::GetRestitution()
    {
        is_modified = true;
        return restitution;
    }

    float EdgeCollider::GetRestitution() const
    {
        return restitution;
    }

    float& EdgeCollider::GetFriction()
    {
        is_modified = true;
        return friction;
    }

    float EdgeCollider::GetFriction() const
    {
        return friction;
    }

    std::vector<Vec2f>& EdgeCollider::_NoModify_GetPoints()
    {
        return points;
    }

    bool& EdgeCollider::_NoModify_GetIsLoop()
    {
        return is_loop;
    }

    Vec2f& EdgeCollider::_NoModify_GetOrigin()
    {
        return origin;
    }

    float& EdgeCollider::_NoModify_GetDensity()
    {
        return density;
    }

    float& EdgeCollider::_NoModify_GetRestitution()
    {
        return restitution;
    }

    float& EdgeCollider::_NoModify_GetFriction()
    {
        return friction;
    }

    void EdgeCollider::_ResetModify()
    {
        is_modified = false;
    }

    bool EdgeCollider::_IsModified() const
    {
        return is_modified;
    }
}