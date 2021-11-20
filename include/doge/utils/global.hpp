#pragma once

#include <concepts>
#include <doge/components.hpp>
#include <doge/utils/Vec2.hpp>
#include <doge/utils/lic.hpp>

namespace doge
{
    struct global
    {
        global(const global&) = delete;

        static Vec2f GetPosition(lic::EntityID eid);

        static Vec2f GetScale(lic::EntityID eid);

        static float GetRotation(lic::EntityID eid);

        template <typename TComp>
        requires requires (TComp c) {
            { c.position } -> std::convertible_to<Vec2f&>;
        }
        static Vec2f GetPosition(const lic::Component<TComp>& c)
        {
            return GetPosition(c.GetEntity()) + c.position;
        }

        template <typename TComp>
        requires requires (TComp c) {
            { c.scale } -> std::convertible_to<Vec2f&>;
        }
        static Vec2f GetScale(const lic::Component<TComp>& c)
        {
            return GetScale(c.GetEntity()) * c.scale;
        }

        template <typename TComp>
        requires requires (TComp c) {
            { c.rotation } -> std::convertible_to<float&>;
        }
        static float GetRotation(const lic::Component<TComp>& c)
        {
            return GetRotation(c.GetEntity()) + c.rotation;
        }
    };
}