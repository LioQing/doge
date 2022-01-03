#include <doge/components/CompoundCollider.hpp>

namespace doge
{
    CompoundCollider::CompoundCollider(
            const std::vector<ConvexCollider>& convex_colliders,
            const std::vector<CircleCollider>& circle_colliders,
            const std::vector<RectangleCollider>& rectangle_colliders
        ) :
        convex_colliders(convex_colliders),
        circle_colliders(circle_colliders),
        rectangle_colliders(rectangle_colliders)
    {
    }

    std::vector<ConvexCollider>& CompoundCollider::GetConvexColliders()
    {
        is_modified = true;
        return convex_colliders;
    }

    const std::vector<ConvexCollider>& CompoundCollider::GetConvexColliders() const
    {
        return convex_colliders;
    }

    std::vector<CircleCollider>& CompoundCollider::GetCircleColliders()
    {
        is_modified = true;
        return circle_colliders;
    }

    const std::vector<CircleCollider>& CompoundCollider::GetCircleColliders() const
    {
        return circle_colliders;
    }

    std::vector<RectangleCollider>& CompoundCollider::GetRectangleColliders()
    {
        is_modified = true;
        return rectangle_colliders;
    }

    const std::vector<RectangleCollider>& CompoundCollider::GetRectangleColliders() const
    {
        return rectangle_colliders;
    }

    std::vector<ConvexCollider>& CompoundCollider::_NoModify_GetConvexColliders()
    {
        return convex_colliders;
    }

    std::vector<CircleCollider>& CompoundCollider::_NoModify_GetCircleColliders()
    {
        return circle_colliders;
    }

    std::vector<RectangleCollider>& CompoundCollider::_NoModify_GetRectangleColliders()
    {
        return rectangle_colliders;
    }

    void CompoundCollider::_ResetModify()
    {
        is_modified = false;
    }

    bool CompoundCollider::_IsModified() const
    {
        return is_modified;
    }
}