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
    struct Engine;
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

        static Rectf GetAABB(const ConvexShape& convex, const Entity& entity);
        static Rectf GetAABB(const Component<ConvexShape>& convex);
        
        static Rectf GetAABB(const CircleShape& circle, const Entity& entity);
        static Rectf GetAABB(const Component<CircleShape>& circle);

        static Rectf GetAABB(const RectangleShape& rectangle, const Entity& entity);
        static Rectf GetAABB(const Component<RectangleShape>& rectangle);

        static Rectf GetAABB(const Sprite& sprite, const Entity& entity);
        static Rectf GetAABB(const Component<Sprite>& sprite);

        static Rectf GetAABB(const Sprite& sprite, const Entity& entity, const Engine& engine);
        static Rectf GetAABB(const Component<Sprite>& sprite, const Engine& engine);

        static Rectf GetAABB(const CustomShape& polygon, const Entity& entity);
        static Rectf GetAABB(const Component<CustomShape>& custom);

        static bool IsEnabled(const Entity& entity);

    private:

        static Rectf GetAABB(const Vec2f& tl, const Vec2f& br, const Vec2f& bl, const Vec2f& tr, const Vec2f& position);
        static Rectf GetAABB(const std::vector<Vec2f>& points, const Vec2f& position);
    };
}