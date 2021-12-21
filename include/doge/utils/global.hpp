#pragma once

#include <concepts>
#include <doge/components.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/utils/lic.hpp>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Entity;
    template <typename TComp>
    struct Component;

    struct global
    {
        global(const global&) = delete;

        static Vec2f GetPosition(const Entity& entity);
        static Vec2f GetPosition(const Component<Position>& position);
        static void SetPosition(Component<Position>& position, const Vec2f& target);

        static Vec2f GetScale(const Entity& entity);
        static Vec2f GetScale(const Component<Scale>& scale);
        static void SetScale(Component<Scale>& scale, const Vec2f& target);

        static float GetRotation(const Entity& entity);
        static float GetRotation(const Component<Rotation>& rotation);
        static void SetRotation(Component<Rotation>& rotation, float target);

        static Rectf GetAABB(const Component<RectangleShape>& rectangle);
        static Rectf GetAABB(const Component<ConvexShape>& convex);
    };
}