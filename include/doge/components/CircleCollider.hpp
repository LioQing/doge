#pragma once

namespace doge
{
    struct CircleCollider
    {
        float radius = 1.f;
        Vec2f origin = Vec2f::Zero();
        
        float density = 0.f;
        float restitution = 0.f;
        float friction = 0.2f;
    };
}