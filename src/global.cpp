#include <doge/utils/global.hpp>

#include <doge/core/Engine.hpp>
#include <doge/core/Entity.hpp>
#include <doge/core/Component.hpp>

namespace doge
{
    Vec2f global::GetPosition(const Entity& entity)
    {
        auto pos = entity.GetIfHasComponentElseDefault<Position>().position;
        for (Entity curr = entity; curr.HasParent(); curr = curr.GetParent())
        {
            pos += curr.GetParent().GetIfHasComponentElseDefault<Position>().position;
        }
        return pos;
    }

    Vec2f global::GetPosition(const Component<Position>& position)
    {
        return GetPosition(position.GetEntity());
    }

    void global::SetPosition(Component<Position>& position, const Vec2f& target)
    {
        position.position += target - GetPosition(position);
    }

    Vec2f global::GetScale(const Entity& entity)
    {
        auto scale = entity.GetIfHasComponentElseDefault<Scale>().scale;
        for (Entity curr = entity; curr.HasParent(); curr = curr.GetParent())
        {
            scale *= curr.GetParent().GetIfHasComponentElseDefault<Scale>().scale;
        }
        return scale;
    }

    Vec2f global::GetScale(const Component<Scale>& scale)
    {
        return GetScale(scale.GetEntity());
    }

    void global::SetScale(Component<Scale>& scale, const Vec2f& target)
    {
        scale.scale *= target / GetScale(scale);
    }

    float global::GetRotation(const Entity& entity)
    {
        auto rot = entity.GetIfHasComponentElseDefault<Rotation>().rotation;
        for (Entity curr = entity; curr.HasParent(); curr = curr.GetParent())
        {
            rot += curr.GetParent().GetIfHasComponentElseDefault<Rotation>().rotation;
        }
        return math::ConstrainAngle(rot);
    }

    float global::GetRotation(const Component<Rotation>& rotation)
    {
        return GetRotation(rotation.GetEntity());
    }

    void global::SetRotation(Component<Rotation>& rotation, float target)
    {
        rotation.rotation += target - GetRotation(rotation);
        rotation.rotation = math::ConstrainAngle(rotation.rotation);
    }
}