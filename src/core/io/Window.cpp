#include <doge/core/io/Window.hpp>

#include <memory>
#include <doge/core.hpp>
#include <doge/components.hpp>
#include <doge/utils.hpp>

namespace doge::io
{
    void Window::CreateWindow(const WindowSettings& settings)
    {
        style = cast::ToSfStyle(settings.style);
        msaa_level = settings.msaa_level;

        sf::ContextSettings context_settings;
        context_settings.antialiasingLevel = msaa_level;

        window.create(sf::VideoMode(settings.size.x, settings.size.y), settings.title, style, context_settings);
        
        if (settings.style == WindowSettings::Style::None)
        {
            SetPosition({ 0, 0 });
            SetSize({ sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height });
        }

        SetFrameRate(settings.fps);
        SetVSyncEnabled(settings.v_sync);

        if (icon.GetSize() != Vec2u::Zero)
        {
            SetIcon(icon);
        }
    }

    void Window::CloseWindow()
    {
        window.close();
    }

    void Window::PollEvent()
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            sf_event(event);
        }
    }

    void Window::Render(const Engine& engine)
    {
        // helper functions
        auto SyncTransformable = []<typename TComp>(sf::Transformable& transform, const TComp& comp, const Entity& entity)
        {
            transform.setOrigin(cast::ToSfVec2(comp.origin));
            transform.setScale(cast::ToSfVec2(global::GetScale(entity)));
            transform.setPosition(cast::ToSfVec2(global::GetPosition(entity)));
            transform.setRotation(cast::ToDegree(global::GetRotation(entity)));
        };

        auto SyncShape = [&]<typename TComp>(sf::Shape& shape, const TComp& comp, const Entity& entity)
        {
            SyncTransformable(shape, comp, entity);
            shape.setFillColor(cast::ToSfColor(comp.color));
            shape.setOutlineColor(cast::ToSfColor(comp.outline_color));
            shape.setOutlineThickness(comp.outline_thickness);
            if (comp.texture_id != "")
            {
                auto& texture = engine.assets.textures.at(comp.texture_id);
                shape.setTexture(&texture.texture);

                Recti rect = comp.texture_rectangle;
                if (!comp.atlas_rectangle_id.empty())
                {
                    rect = texture.atlas_rectangles.at(comp.atlas_rectangle_id);
                }
                else if (rect.width == 0 && rect.height == 0)
                {
                    rect.SetSize(texture.GetSize());
                }

                shape.setTextureRect(cast::ToSfRect(rect));
            }
            else
            {
                shape.setTexture(nullptr);
            }
        };

        auto EmplaceDrawables = [&]<typename TSfShape, typename TComp>(const DrawableKey& key, Component<TComp>& comp)
        {
            // draws_layers
            auto layer_comp = comp.GetEntity().GetIfHasComponentElseDefault<Layer>();
            if (layer_comp.layers.size() < 1)
                throw std::invalid_argument("Layer component of Entity have no layer information");
            int layer = *layer_comp.layers.rbegin();

            auto layer_itr = draws_layers.find(key);
            if (layer_itr == draws_layers.end())
            {
                auto [layer_itr_, success] = draws_layers.emplace(key, layer);
                if (!success)
                {
                    throw std::invalid_argument("Failed to emplace layer info for entity");
                }
                layer_itr = std::move(layer_itr_);
                comp.OnRemoval([&, val_key = key](){ draws_layers.erase(val_key); });
            }
            else
            {
                layer_itr->second = layer;
            }

            // drawables
            auto draw_itr = drawables.find(key);
            if (draw_itr != drawables.end())
                return draw_itr;
            
            auto [draw_itr_, success] = drawables.emplace(key, static_cast<std::unique_ptr<sf::Drawable>>(std::make_unique<TSfShape>()));
            if (!success)
            {
                throw std::invalid_argument("More than one shape is found in entity");
            }
            draw_itr = std::move(draw_itr_);
            comp.OnRemoval([&, val_key = key](){ drawables.erase(val_key); });
            return draw_itr;
        };

        auto InAnyViewHelper = [&]<typename TComp>(
            const TComp& comp, 
            const Entity& entity, 
            const DrawableKey& key)
        {
            auto is_in_any_view = false;
            Rectf aabb = global::GetAABB(comp, entity);

            if (aabb.width < 0.f)
            {
                aabb.width = std::abs(aabb.width);
                aabb.left -= aabb.width;
            }

            if (aabb.height < 0.f)
            {
                aabb.height = std::abs(aabb.height);
                aabb.top -= aabb.height;
            }

            for (auto cam_entity : engine.Select<Camera>().Entities())
            {
                auto& [view_ptr, draw_keys, render_order] = views_draws.at(cam_entity.id);
                
                bool is_in_view = 
                    aabb.left + aabb.width > view_ptr->getCenter().x - view_ptr->getSize().x / 2.f &&
                    aabb.top + aabb.height > view_ptr->getCenter().y - view_ptr->getSize().y / 2.f &&
                    aabb.left < view_ptr->getCenter().x + view_ptr->getSize().x / 2.f &&
                    aabb.top < view_ptr->getCenter().y + view_ptr->getSize().y / 2.f;

                is_in_any_view = is_in_any_view || is_in_view;

                auto draw_key_itr = draw_keys.find(key);
                if (draw_key_itr != draw_keys.end() && !is_in_view)
                {
                    draw_keys.erase(key);
                }
                else if (draw_key_itr == draw_keys.end() && is_in_view)
                {
                    draw_keys.emplace(key);
                }
            }

            return is_in_any_view;
        };

        // view
        for (auto [entity, cam] : engine.Select<Camera>().EntitiesAndComponents())
        {
            auto view_itr = views_draws.find(entity.id);
            if (view_itr == views_draws.end())
            {
                view_itr = views_draws.emplace(entity.id, ViewInfo(std::make_unique<sf::View>(), std::set<DrawableKey>(), cam.render_order)).first;
                cam.OnRemoval([&, eid = entity.id](){ views_draws.erase(eid); });
            }

            auto& view = std::get<0>(view_itr->second);
            
            view->setCenter(cast::ToSfVec2(global::GetPosition(entity)));
            if (cam.size == Vec2f::Zero)
            {
                view->setSize(cast::ToSfVec2(engine.window.GetAutoResizedCameraSize(cam) * global::GetScale(entity)));
            }
            else
            {
                view->setSize(cast::ToSfVec2(cam.size * global::GetScale(entity)));
            }
            view->setRotation(cast::ToDegree(global::GetRotation(entity)));
            view->setViewport(cast::ToSfRect(cam.port));
        }

        // clearing views_draws
        for (auto& [entity, view_draw] : views_draws)
        {
            std::get<1>(view_draw).clear();
        }

        // convex shape
        auto SyncConvex = [&](sf::ConvexShape& convex_shape, const ConvexShape& convex_comp, const Entity& entity)
        {
            SyncShape(convex_shape, convex_comp, entity);
            convex_shape.setPointCount(convex_comp.points.size());
            for (std::size_t i = 0; i < convex_comp.points.size(); ++i)
            {
                convex_shape.setPoint(i, cast::ToSfVec2(convex_comp.points.at(i)));
            }
        };

        auto UpdateConvex = [&]<typename TComp>(Component<TComp>& comp, const ConvexShape& convex_comp, const Entity& entity, std::size_t index)
        {
            auto key = DrawableKey(entity, DrawableType::Convex, index);
            auto draw_itr = EmplaceDrawables.template operator()<sf::ConvexShape>(key, comp);

            if (InAnyViewHelper(convex_comp, entity, key))
            {
                SyncConvex(static_cast<sf::ConvexShape&>(*draw_itr->second), convex_comp, entity);
            }
        };

        for (auto [entity, convex_comp] : engine.Select<ConvexShape>().EntitiesAndComponents())
        {
            UpdateConvex(convex_comp, convex_comp, entity, 0);
        }

        // circle shape
        auto SyncCircle = [&](sf::CircleShape& circle_shape, const CircleShape& circle_comp, const Entity& entity)
        {
            SyncShape(circle_shape, circle_comp, entity);
            circle_shape.setRadius(circle_comp.radius);
            circle_shape.setPointCount(circle_comp.point_count);
        };

        auto UpdateCircle = [&]<typename TComp>(Component<TComp>& comp, const CircleShape& circle_comp, const Entity& entity, std::size_t index)
        {
            auto key = DrawableKey(entity, DrawableType::Circle, index);
            auto draw_itr = EmplaceDrawables.template operator()<sf::CircleShape>(key, comp);
            
            if (InAnyViewHelper(circle_comp, entity, key))
            {
                SyncCircle(static_cast<sf::CircleShape&>(*draw_itr->second), circle_comp, entity);
            }
        };

        for (auto [entity, circle_comp] : engine.Select<CircleShape>().EntitiesAndComponents())
        {
            UpdateCircle(circle_comp, circle_comp, entity, 0);
        }

        // rectangle shape
        auto SyncRectangle = [&](sf::RectangleShape& rectangle_shape, const RectangleShape& rectangle_comp, const Entity& entity)
        {
            SyncShape(rectangle_shape, rectangle_comp, entity);
            rectangle_shape.setSize(cast::ToSfVec2(rectangle_comp.size));
        };

        auto UpdateRectangle = [&]<typename TComp>(Component<TComp>& comp, const RectangleShape& rectangle_comp, const Entity& entity, std::size_t index)
        {
            auto key = DrawableKey(entity, DrawableType::Rectangle, index);
            auto draw_itr = EmplaceDrawables.template operator()<sf::RectangleShape>(key, comp);

            if (InAnyViewHelper(rectangle_comp, entity, key))
            {
                SyncRectangle(static_cast<sf::RectangleShape&>(*draw_itr->second), rectangle_comp, entity);
            }
        };

        for (auto [entity, rectangle_comp] : engine.Select<RectangleShape>().EntitiesAndComponents())
        {
            UpdateRectangle(rectangle_comp, rectangle_comp, entity, 0);
        }

        // sprite
        auto SyncSprite = [&](sf::Sprite& sprite, const Sprite& sprite_comp, const Entity& entity)
        {
            SyncTransformable(sprite, sprite_comp, entity);

            auto& texture = engine.assets.textures.at(sprite_comp.texture_id);

            Recti rect = sprite_comp.texture_rectangle;
            if (!sprite_comp.atlas_rectangle_id.empty())
            {
                rect = texture.atlas_rectangles.at(sprite_comp.atlas_rectangle_id);
            }
            else
            {
                rect = math::AutoSize(rect, texture.GetSize());
            }

            auto scale = sprite_comp.size / rect.GetSize();
            scale = math::AutoSize(scale, Vec2f::One);
            sprite.setScale(sprite.getScale().x * scale.x, sprite.getScale().y * scale.y);
            sprite.setOrigin(sprite.getOrigin().x / scale.x, sprite.getOrigin().y / scale.y);

            sprite.setTexture(texture.texture);

            sprite.setTextureRect(cast::ToSfRect(rect));
            sprite.setColor(cast::ToSfColor(sprite_comp.color));
        };

        auto UpdateSprite = [&]<typename TComp>(Component<TComp>& comp, const Sprite& sprite_comp, const Entity& entity, std::size_t index)
        {
            auto key = DrawableKey(entity, DrawableType::SpriteType, index);
            auto draw_itr = EmplaceDrawables.template operator()<sf::Sprite>(key, comp);

            if (InAnyViewHelper(sprite_comp, entity, key))
            {
                SyncSprite(static_cast<sf::Sprite&>(*draw_itr->second), sprite_comp, entity);
            }
        };

        for (auto [entity, sprite_comp] : engine.Select<Sprite>().EntitiesAndComponents())
        {
            UpdateSprite(sprite_comp, sprite_comp, entity, 0);
        }

        // polygon shape
        auto SyncPolygon = [&](DrawableVertices& vertices, const PolygonShape& polygon_comp, const Entity& entity)
        {
            auto size = polygon_comp.vertices.size();

            vertices.type = cast::ToSfPolygonType(polygon_comp.type);

            if (vertices.vertices.size() < polygon_comp.vertices.size())
            {
                vertices.vertices.resize(size);
            }

            for (std::size_t i = 0; i < size; ++i)
            {
                auto& vertex = polygon_comp.vertices.at(i);
                vertices.vertices.at(i).position = 
                    cast::ToSfVec2(((vertex.position - polygon_comp.origin) * global::GetScale(entity)).Rotated(global::GetRotation(entity))
                    + global::GetPosition(entity));
                vertices.vertices.at(i).color = cast::ToSfColor(vertex.color);
                vertices.vertices.at(i).texCoords = cast::ToSfVec2(vertex.texture_coordinates);
            }

            if (polygon_comp.texture_id != "")
            {
                vertices.texture = &engine.assets.textures.at(polygon_comp.texture_id).texture;
            }
            else
            {
                vertices.texture = nullptr;
            }
        };

        auto UpdatePolygon = [&]<typename TComp>(Component<TComp>& comp, const PolygonShape& polygon_comp, const Entity& entity, std::size_t index)
        {
            auto key = DrawableKey(entity, DrawableType::Polygon, index);
            auto draw_itr = EmplaceDrawables.template operator()<DrawableVertices>(key, comp);

            if (InAnyViewHelper(polygon_comp, entity, key))
            {
                SyncPolygon(static_cast<DrawableVertices&>(*draw_itr->second), polygon_comp, entity);
            }
        };

        for (auto [entity, polygon_comp] : engine.Select<PolygonShape>().EntitiesAndComponents())
        {
            UpdatePolygon(polygon_comp, polygon_comp, entity, 0);
        }

        // compound shape
        for (auto [entity, compound_comp] : engine.Select<CompoundSprite>().EntitiesAndComponents())
        {
            // convex sub shape
            for (std::size_t i = 0; i < compound_comp.convex_shapes.size(); ++i)
            {
                UpdateConvex(compound_comp, compound_comp.convex_shapes.at(i), entity, i);
            }

            // circle sub shape
            for (std::size_t i = 0; i < compound_comp.circle_shapes.size(); ++i)
            {
                UpdateCircle(compound_comp, compound_comp.circle_shapes.at(i), entity, i);
            }

            // rectangle sub shape
            for (std::size_t i = 0; i < compound_comp.rectangle_shapes.size(); ++i)
            {
                UpdateRectangle(compound_comp, compound_comp.rectangle_shapes.at(i), entity, i);
            }

            // sub sprites
            for (std::size_t i = 0; i < compound_comp.sprites.size(); ++i)
            {
                UpdateSprite(compound_comp, compound_comp.sprites.at(i), entity, i);
            }
        }

        // layers_draws
        std::map<std::int32_t, std::set<const DrawableKey*>> layers_draws;
        
        for (auto& [draw, layer] : draws_layers)
        {
            layers_draws[layer].emplace(&draw);
        }

        // cam orders
        std::map<std::int32_t, std::unordered_map<EntityID, std::reference_wrapper<ViewInfo>>> orders_views;

        for (auto& [cam_id, view_info] : views_draws)
        {
            orders_views[std::get<2>(view_info)].emplace(cam_id, std::ref(view_info));
        }

        // draw
        window.clear(cast::ToSfColor(background_color));
        for (auto& [order, view_infos] : orders_views)
        {
            for (auto& [cam_id, view_info] : view_infos)
            {
                auto& [view, draw_keys, _] = view_info.get();

                window.setView(*view);

                // draw
                if (engine.GetEntity(cam_id).HasComponent<Layer>())
                {
                    auto& cam_layers = engine.GetEntity(cam_id).GetComponent<Layer>().layers;

                    for (auto& [layer, layer_draw_keys] : layers_draws)
                    {
                        if (!cam_layers.contains(layer))
                            continue;

                        for (auto& draw_key : layer_draw_keys)
                        {
                            if (draw_keys.contains(*draw_key))
                                window.draw(*drawables.at(*draw_key));
                        }
                    }
                }
                else
                {
                    for (auto& draw_key : layers_draws.at(0))
                    {
                        if (draw_keys.contains(*draw_key))
                            window.draw(*drawables.at(*draw_key));
                    }
                }
            }
        }
    }

    void Window::Display()
    {
        window.display();
    }

    Image Window::TakeScreenshot() const
    {
        sf::Vector2u size = window.getSize();
        sf::Texture texture;
        texture.create(size.x, size.y);
        texture.update(window);
        sf::Image screenshot = texture.copyToImage();

        return Image(screenshot);
    }

    // delta time

    void Window::StartDeltaClock()
    {
        clock.restart();
    }

    float Window::GetDeltaTimeRestart()
    {
        return clock.restart().asMicroseconds() / 1000.f;
    }

    // properties

    void Window::ApplySettings(const WindowSettings& settings)
    {
        if (!IsOpen())
            return;

        if (settings.style != style || settings.msaa_level != msaa_level)
        {
            CreateWindow(settings);
            return;
        }
        
        SetSize(settings.size);
        SetTitle(settings.title);
        SetFrameRate(settings.fps);
        SetVSyncEnabled(settings.v_sync);
    }

    void Window::SetFrameRate(std::uint32_t frame_rate)
    {
        window.setFramerateLimit(frame_rate);
    }

    void Window::SetTitle(const std::string& title)
    {
        window.setTitle(title);
    }

    void Window::SetIcon(const Image& icon)
    {
        window.setIcon(icon.GetSize().x, icon.GetSize().y, icon.GetPixelPtr());
    }

    void Window::SetVisible(bool visible)
    {
        window.setVisible(visible);
    }

    void Window::SetVSyncEnabled(bool enabled)
    {
        window.setVerticalSyncEnabled(enabled);
    }

    void Window::SetMouseCursorVisible(bool visible)
    {
        window.setMouseCursorVisible(visible);
    }

    void Window::SetMouseCursorGrabbed(bool grabbed)
    {
        window.setMouseCursorGrabbed(grabbed);
    }

    void Window::SetMouseCursor(const io::Cursor& cursor)
    {
        window.setMouseCursor(cursor.cursor);
    }

    bool Window::IsOpen() const
    {
        return window.isOpen();
    }

    Vec2i Window::GetPosition() const
    {
        return cast::FromSfVec2(window.getPosition());
    }

    void Window::SetPosition(const Vec2i& pos)
    {
        window.setPosition(cast::ToSfVec2(pos));
    }

    Vec2u Window::GetSize() const
    {
        return cast::FromSfVec2(window.getSize());
    }

    void Window::SetSize(const Vec2u& size)
    {
        window.setSize(cast::ToSfVec2(size));
    }

    void Window::RequestFocus()
    {
        window.requestFocus();
    }

    bool Window::HasFocus() const
    {
        return window.hasFocus();
    }

    Vec2i Window::GetClientAreaPosition() const
    {
        using m = io::Mouse;

        return m::GetPosition(*this) - m::GetPosition();
    }

    Vec2i Window::GetMousePosition() const
    {
        return cast::FromSfVec2(sf::Mouse::getPosition(window));
    }

    void Window::SetMousePosition(const Vec2i& position) const
    {
        sf::Mouse::setPosition(cast::ToSfVec2(position), window);
    }
}