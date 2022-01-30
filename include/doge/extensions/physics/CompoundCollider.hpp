#pragma once

#include <vector>
#include <doge/extensions/physics/ConvexCollider.hpp>
#include <doge/extensions/physics/CircleCollider.hpp>
#include <doge/extensions/physics/RectangleCollider.hpp>

namespace doge::physics
{
    struct CompoundCollider
    {
        std::vector<ConvexCollider> convex_colliders;
        std::vector<CircleCollider> circle_colliders;
        std::vector<RectangleCollider> rectangle_colliders;
        
        bool is_enabled = true;
        bool is_trigger = false;
        
        // doge::Physics check this every frame
        // after changes are applied, it will be reset to false
        bool apply_changes = false;

        template <typename... Colliders>
        static CompoundCollider Create(Colliders&&... colliders)
        {
            CompoundCollider comp_coll;

            auto emplace_coll =
            [&]<typename C>(const C& coll)
            {
                if constexpr (std::is_constructible_v<ConvexCollider, C>)
                    comp_coll.convex_colliders.emplace_back(coll);
                else if constexpr (std::is_constructible_v<CircleCollider, C>)
                    comp_coll.circle_colliders.emplace_back(coll);
                else if constexpr (std::is_constructible_v<RectangleCollider, C>)
                    comp_coll.rectangle_colliders.emplace_back(coll);
                else
                    static_assert(false, "Invalid collider type");
            };

            (emplace_coll(colliders), ...);

            return comp_coll;
        }
    };
}