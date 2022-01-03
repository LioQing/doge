#include <doge/utils/global.hpp>

#include <doge/core/Engine.hpp>
#include <doge/core/Entity.hpp>
#include <doge/core/Component.hpp>

namespace doge
{
    Vec2f global::GetPosition(const Entity& entity)
    {
        auto pos = entity.GetIfHasComponentElseDefault<Position>().Get();
        for (Entity curr = entity; curr.HasParent(); curr = curr.GetParent())
        {
            pos += curr.GetParent().GetIfHasComponentElseDefault<Position>().Get();
        }
        return pos;
    }

    Vec2f global::GetPosition(const Component<Position>& position)
    {
        return GetPosition(position.GetEntity());
    }

    void global::SetPosition(Component<Position>& position, const Vec2f& target)
    {
        position.Get() += target - GetPosition(position);
    }

    void global::_NoModify_SetPosition(Component<Position>& position, const Vec2f& target)
    {
        position._NoModify_Get() += target - GetPosition(position);
    }

    Vec2f global::GetScale(const Entity& entity)
    {
        auto scale = entity.GetIfHasComponentElseDefault<Scale>().Get();
        for (Entity curr = entity; curr.HasParent(); curr = curr.GetParent())
        {
            scale *= curr.GetParent().GetIfHasComponentElseDefault<Scale>().Get();
        }
        return scale;
    }

    Vec2f global::GetScale(const Component<Scale>& scale)
    {
        return GetScale(scale.GetEntity());
    }

    void global::SetScale(Component<Scale>& scale, const Vec2f& target)
    {
        scale.Get() *= target / GetScale(scale);
    }

    void global::_NoModify_SetScale(Component<Scale>& scale, const Vec2f& target)
    {
        scale._NoModify_Get() *= target / GetScale(scale);
    }

    float global::GetRotation(const Entity& entity)
    {
        auto rot = entity.GetIfHasComponentElseDefault<Rotation>().Get();
        for (Entity curr = entity; curr.HasParent(); curr = curr.GetParent())
        {
            rot += curr.GetParent().GetIfHasComponentElseDefault<Rotation>().Get();
        }
        return math::ConstrainAngle(rot);
    }

    float global::GetRotation(const Component<Rotation>& rotation)
    {
        return GetRotation(rotation.GetEntity());
    }

    void global::SetRotation(Component<Rotation>& rotation, float target)
    {
        rotation.Get() += target - GetRotation(rotation);
        rotation.Get() = math::ConstrainAngle(rotation.Get());
    }
    
    void global::_NoModify_SetRotation(Component<Rotation>& rotation, float target)
    {
        rotation._NoModify_Get() += target - GetRotation(rotation);
        rotation._NoModify_Get() = math::ConstrainAngle(rotation._NoModify_Get());
    }

    Rectf global::GetAABB(const ConvexShape& convex, const Entity& entity)
    {
        std::vector<Vec2f> points;
        std::transform(convex.GetPoints().begin(), convex.GetPoints().end(), std::back_inserter(points),
        [&](const Vec2f& v){ return ((v - convex.GetOrigin()) * GetScale(entity)).Rotate(GetRotation(entity)); });

        Rectf aabb(points.front().x, points.front().y, points.front().x, points.front().y);
        for (auto& point : points)
        {
            if (point.x < aabb.left) aabb.left = point.x;
            if (point.y < aabb.top) aabb.top = point.y;
            if (point.x > aabb.width) aabb.width = point.x;
            if (point.y > aabb.height) aabb.height = point.y;
        }

        aabb.width -= aabb.left;
        aabb.height -= aabb.top;
        aabb.left += GetPosition(entity).x;
        aabb.top += GetPosition(entity).y;

        return aabb;
    }

    Rectf global::GetAABB(const Component<ConvexShape>& convex)
    {
        return GetAABB(convex, convex.GetEntity());
    }

    Rectf global::GetAABB(const CircleShape& circle, const Entity& entity)
    {
        return Rectf(
            -circle.GetOrigin().x * GetScale(entity).x + GetPosition(entity).x,
            -circle.GetOrigin().y * GetScale(entity).y + GetPosition(entity).y,
            circle.GetRadius() * 2.f * GetScale(entity).x,
            circle.GetRadius() * 2.f * GetScale(entity).y
        );
    }

    Rectf global::GetAABB(const Component<CircleShape>& circle)
    {
        return GetAABB(circle, circle.GetEntity());
    }

    Rectf global::GetAABB(const RectangleShape& rectangle, const Entity& entity)
    {
        auto tl = ((Vec2f::Zero() - rectangle.GetOrigin()) * GetScale(entity)).Rotate(GetRotation(entity));
        auto br = ((rectangle.GetSize() - rectangle.GetOrigin()) * GetScale(entity)).Rotate(GetRotation(entity));
        auto bl = Vec2f(tl.x, br.y).Rotate(GetRotation(entity));
        auto tr = Vec2f(br.x, tl.y).Rotate(GetRotation(entity));

        Rectf aabb(
            std::min({ tl.x, br.x, bl.x, tr.x }), 
            std::min({ tl.y, br.y, bl.y, tr.y }), 
            std::max({ tl.x, br.x, bl.x, tr.x }), 
            std::max({ tl.y, br.y, bl.y, tr.y })
        );

        aabb.width -= aabb.left;
        aabb.height -= aabb.top;
        aabb.left += GetPosition(entity).x;
        aabb.top += GetPosition(entity).y;

        return aabb;
    }

    Rectf global::GetAABB(const Component<RectangleShape>& rectangle)
    {
        return GetAABB(rectangle, rectangle.GetEntity());
    }
}