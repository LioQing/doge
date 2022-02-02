#include <doge/utils/global.hpp>

#include <doge/core/Engine.hpp>
#include <doge/core/Entity.hpp>
#include <doge/core/Component.hpp>
#include <doge/core/io/Texture.hpp>

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
        auto scale = GetScale(entity);
        auto rotation = GetRotation(entity);

        auto end_x = (Vec2f(circle.radius, 0).Rotated(rotation) * scale).Magnitude();
        auto end_y = (Vec2f(0, circle.radius).Rotated(rotation) * scale).Magnitude();
        return Rectf(
            ((-circle.origin + Vec2f(circle.radius, circle.radius)) * scale).Rotated(rotation) - Vec2f(end_x, end_y) + GetPosition(entity),
            Vec2f(end_x, end_y) * 2.f
        );
    }

    Rectf global::GetAABB(const Component<CircleShape>& circle)
    {
        return GetAABB(circle, circle.GetEntity());
    }

    Rectf global::GetAABB(const RectangleShape& rectangle, const Entity& entity)
    {
        auto tl = ((-rectangle.origin) * GetScale(entity));
        auto br = ((rectangle.size - rectangle.origin) * GetScale(entity));

        return GetAABB(
            tl.Rotated(GetRotation(entity)),
            br.Rotated(GetRotation(entity)),
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
        if (sprite.size.x == 0 && sprite.texture_rectangle.GetSize().x == 0 ||
            sprite.size.y == 0 && sprite.texture_rectangle.GetSize().y == 0 )
            throw std::invalid_argument("Sprite::size has member of value 0, GetAABB should be called with doge::Engine");

        auto size = math::AutoSize(sprite.size, sprite.texture_rectangle.GetSize());

        auto tl = (-sprite.origin) * GetScale(entity);
        auto br = (size - sprite.origin) * GetScale(entity);

        return GetAABB(
            tl.Rotated(GetRotation(entity)),
            br.Rotated(GetRotation(entity)),
            Vec2f(tl.x, br.y).Rotated(GetRotation(entity)),
            Vec2f(br.x, tl.y).Rotated(GetRotation(entity)),
            GetPosition(entity)
        );
    }
    
    Rectf global::GetAABB(const Component<Sprite>& sprite)
    {
        return GetAABB(sprite, sprite.GetEntity());
    }

    Rectf global::GetAABB(const Sprite& sprite, const Entity& entity, const Engine& engine)
    {
        auto size = math::AutoSize(sprite.size, math::AutoSize(sprite.texture_rectangle.GetSize(), engine.assets.GetTexture(sprite.texture_id).GetSize()));
        if (!sprite.atlas_rectangle_id.empty())
            size = engine.assets.GetTexture(sprite.texture_id).atlas_rectangles.at(sprite.atlas_rectangle_id).GetSize();

        auto tl = (-sprite.origin) * GetScale(entity);
        auto br = (size - sprite.origin) * GetScale(entity);

        return GetAABB(
            tl.Rotated(GetRotation(entity)),
            br.Rotated(GetRotation(entity)),
            Vec2f(tl.x, br.y).Rotated(GetRotation(entity)),
            Vec2f(br.x, tl.y).Rotated(GetRotation(entity)),
            GetPosition(entity)
        );
    }
    
    Rectf global::GetAABB(const Component<Sprite>& sprite, const Engine& engine)
    {
        return GetAABB(sprite, sprite.GetEntity(), engine);
    }

    Rectf global::GetAABB(const PolygonShape& polygon, const Entity& entity)
    {
        std::vector<Vec2f> points;
        std::transform(polygon.vertices.begin(), polygon.vertices.end(), std::back_inserter(points),
        [&](const PolygonShape::Vertex& v){ return ((v.position - polygon.origin) * GetScale(entity)).Rotated(GetRotation(entity)); });

        return GetAABB(points, GetPosition(entity));
    }

    Rectf global::GetAABB(const Component<PolygonShape>& polygon)
    {
        return GetAABB(polygon, polygon.GetEntity());
    }

    bool global::IsEnabled(const Entity& entity)
    {
        Entity curr_entity = entity;

        while (curr_entity.HasParent())
        {
            if (!curr_entity.GetComponent<EntityInfo>().enabled)
                return false;
            
            curr_entity = curr_entity.GetParent();
        }

        return curr_entity.GetComponent<EntityInfo>().enabled;
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