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

    Rectf global::GetAABB(const ConvexShape& convex, const Entity& entity)
    {
        std::vector<Vec2f> points;
        std::transform(convex.points.begin(), convex.points.end(), std::back_inserter(points),
        [&](const Vec2f& v){ return ((v - convex.origin) * GetScale(entity)).Rotated(GetRotation(entity)); });

        return GetAABB(points, GetPosition(entity));
    }

    Rectf global::GetAABB(const Component<ConvexShape>& convex)
    {
        return GetAABB(convex, convex.GetEntity());
    }

    Rectf global::GetAABB(const CircleShape& circle, const Entity& entity)
    {
        return Rectf(
            -circle.origin.x * GetScale(entity).x + GetPosition(entity).x,
            -circle.origin.y * GetScale(entity).y + GetPosition(entity).y,
            circle.radius * 2.f * GetScale(entity).x,
            circle.radius * 2.f * GetScale(entity).y
        );
    }

    Rectf global::GetAABB(const Component<CircleShape>& circle)
    {
        return GetAABB(circle, circle.GetEntity());
    }

    Rectf global::GetAABB(const RectangleShape& rectangle, const Entity& entity)
    {
        auto tl = ((Vec2f::Zero() - rectangle.origin) * GetScale(entity)).Rotated(GetRotation(entity));
        auto br = ((rectangle.size - rectangle.origin) * GetScale(entity)).Rotated(GetRotation(entity));

        return GetAABB(
            tl, br,
            Vec2f(tl.x, br.y).Rotated(GetRotation(entity)),
            Vec2f(br.x, tl.y).Rotated(GetRotation(entity)),
            GetPosition(entity)
        );
    }

    Rectf global::GetAABB(const Component<RectangleShape>& rectangle)
    {
        return GetAABB(rectangle, rectangle.GetEntity());
    }

    Rectf global::GetAABB(const Sprite& sprite, const Entity& entity)
    {
        auto tl = ((Vec2f::Zero() - sprite.origin) * GetScale(entity)).Rotated(GetRotation(entity));
        auto br = ((Vec2f(sprite.texture_rectangle.width, sprite.texture_rectangle.height) - sprite.origin) * GetScale(entity)).Rotated(GetRotation(entity));

        return GetAABB(
            tl, br,
            Vec2f(tl.x, br.y).Rotated(GetRotation(entity)),
            Vec2f(br.x, tl.y).Rotated(GetRotation(entity)),
            GetPosition(entity)
        );
    }
    
    Rectf global::GetAABB(const Component<Sprite>& sprite)
    {
        return GetAABB(sprite, sprite.GetEntity());
    }

    Rectf global::GetAABB(const CustomShape& custom, const Entity& entity)
    {
        std::vector<Vec2f> points;
        std::transform(custom.vertices.begin(), custom.vertices.end(), std::back_inserter(points),
        [&](const CustomShape::Vertex& v){ return ((v.position - custom.origin) * GetScale(entity)).Rotated(GetRotation(entity)); });

        return GetAABB(points, GetPosition(entity));
    }

    Rectf global::GetAABB(const Component<CustomShape>& custom)
    {
        return GetAABB(custom, custom.GetEntity());
    }

    Rectf global::GetAABB(const Vec2f& tl, const Vec2f& br, const Vec2f& bl, const Vec2f& tr, const Vec2f& position)
    {
        Rectf aabb(
            std::min({ tl.x, br.x, bl.x, tr.x }), 
            std::min({ tl.y, br.y, bl.y, tr.y }), 
            std::max({ tl.x, br.x, bl.x, tr.x }), 
            std::max({ tl.y, br.y, bl.y, tr.y })
        );

        aabb.width -= aabb.left;
        aabb.height -= aabb.top;
        aabb.left += position.x;
        aabb.top += position.y;

        return aabb;
    }

    Rectf global::GetAABB(const std::vector<Vec2f>& points, const Vec2f& position)
    {
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
        aabb.left += position.x;
        aabb.top += position.y;

        return aabb;
    }
}