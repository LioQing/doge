#pragma once

#include <vector>
#include <doge/components/ConvexCollider.hpp>
#include <doge/components/CircleCollider.hpp>
#include <doge/components/RectangleCollider.hpp>

namespace doge
{
    class CompoundCollider
    {
        std::vector<ConvexCollider> convex_colliders;
        std::vector<CircleCollider> circle_colliders;
        std::vector<RectangleCollider> rectangle_colliders;

        bool is_modified = true;

    public:

        CompoundCollider(const CompoundCollider&) = default;
        CompoundCollider(
            const std::vector<ConvexCollider>& convex_colliders = std::vector<ConvexCollider>(),
            const std::vector<CircleCollider>& circle_colliders = std::vector<CircleCollider>(),
            const std::vector<RectangleCollider>& rectangle_colliders = std::vector<RectangleCollider>()
        );

        std::vector<ConvexCollider>& GetConvexColliders();
        const std::vector<ConvexCollider>& GetConvexColliders() const;

        std::vector<CircleCollider>& GetCircleColliders();
        const std::vector<CircleCollider>& GetCircleColliders() const;

        std::vector<RectangleCollider>& GetRectangleColliders();
        const std::vector<RectangleCollider>& GetRectangleColliders() const;

        std::vector<ConvexCollider>& _NoModify_GetConvexColliders();
        std::vector<CircleCollider>& _NoModify_GetCircleColliders();
        std::vector<RectangleCollider>& _NoModify_GetRectangleColliders();
        void _ResetModify();
        bool _IsModified() const;
    };
}