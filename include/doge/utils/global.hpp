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

        template <typename TComp>
        requires requires (TComp c) {
            { c.position } -> std::convertible_to<Vec2f&>;
        }
        static Vec2f GetPosition(const lic::Component<TComp>& c)
        {
            return c.GetEntity().GetIfHasComponentElseDefault<Position>().position + c.position;
        }

        template <typename TComp>
        requires requires (TComp c) {
            { c.scale } -> std::convertible_to<Vec2f&>;
        }
        static Vec2f GetScale(const lic::Component<TComp>& c)
        {
            return c.GetEntity().GetIfHasComponentElseDefault<Scale>().scale + c.scale;
        }

        template <typename TComp>
        requires requires (TComp c) {
            { c.rotation } -> std::convertible_to<float&>;
        }
        static float GetRotation(const lic::Component<TComp>& c)
        {
            return c.GetEntity().GetIfHasComponentElseDefault<Rotation>().rotation + c.rotation;
        }
    };
}