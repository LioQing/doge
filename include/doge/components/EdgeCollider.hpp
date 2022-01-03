#pragma once

#include <vector>
#include <doge/utils/Vec2.hpp>

namespace doge
{
    class EdgeCollider
    {
        // collision only happen on right side of edge
        std::vector<Vec2f> points;

        bool is_loop = false;
        Vec2f origin = Vec2f::Zero();
        
        float density = 0.f;
        float restitution = 0.f;
        float friction = 0.2f;

        bool is_modified = true;

    public:

        EdgeCollider(const EdgeCollider&) = default;
        EdgeCollider(
            const std::vector<Vec2f>& points = std::vector<Vec2f>(),
            bool is_loop = false,
            const Vec2f& origin = Vec2f::Zero(),
            float density = 0.f,
            float restitution = 0.f,
            float friction = 0.2f
        );

        std::vector<Vec2f>& GetPoints();
        const std::vector<Vec2f>& GetPoints() const;

        bool& GetIsLoop();
        bool GetIsLoop() const;

        Vec2f& GetOrigin();
        const Vec2f& GetOrigin() const;
        
        float& GetDensity();
        float GetDensity() const;
        
        float& GetRestitution();
        float GetRestitution() const;
        
        float& GetFriction();
        float GetFriction() const;

        std::vector<Vec2f>& _NoModify_GetPoints();
        bool& _NoModify_GetIsLoop();
        Vec2f& _NoModify_GetOrigin();
        float& _NoModify_GetDensity();
        float& _NoModify_GetRestitution();
        float& _NoModify_GetFriction();
        void _ResetModify();
        bool _IsModified() const;
    };
}