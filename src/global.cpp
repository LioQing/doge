#include <doge/utils/global.hpp>

#include <doge/core/Engine.hpp>
#include <doge/core/Entity.hpp>

namespace doge
{
    Vec2f global::GetPosition(Entity& entity)
    {
        auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
        for (Entity parent = entity; parent.HasParent(); parent = parent.GetParent())
        {
            pos += parent.GetIfHasComponentElseDefault<Position>().position;
        }
        return pos;
    }

    Vec2f global::GetScale(Entity& entity)
    {
        auto scale = entity.GetIfHasComponentElseDefault<Scale>().scale;
        for (Entity parent = entity; parent.HasParent(); parent = parent.GetParent())
        {
            scale *= parent.GetIfHasComponentElseDefault<Scale>().scale;
        }
        return scale;
    }

    float global::GetRotation(Entity& entity)
    {
        auto rot = entity.GetIfHasComponentElseDefault<Rotation>().rotation;
        for (Entity parent = entity; parent.HasParent(); parent = parent.GetParent())
        {
            rot += parent.GetIfHasComponentElseDefault<Rotation>().rotation;
        }
        return math::ConstrainAngle(rot);
    }
}