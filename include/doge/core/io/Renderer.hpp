#pragma once

#include <SFML/Graphics.hpp>
#include <doge/components.hpp>
#include <doge/core/Engine.hpp>
#include <doge/core/Component.hpp>
#include <doge/core/io/Window.hpp>
#include <doge/utils.hpp>

namespace doge::io
{
    struct Renderer
    {
        Renderer(const Renderer&) = delete;

        template <typename TComp>
        static void RenderConvexShape(Engine& engine, Component<TComp>& comp, const ConvexShape& convex_comp, std::size_t index)
        {
            auto& window = engine.window.window_io;
            auto entity = comp.GetEntity();
            auto key = Window::DrawableKey(entity, Window::DrawableType::Convex, index);
            auto draw_itr = Renderer::EmplaceDrawables<sf::ConvexShape>(window, key, comp);
            auto& convex_shape = static_cast<sf::ConvexShape&>(*draw_itr->second);

            if (Renderer::InAnyViewHelper(window, engine, convex_comp, entity, key))
            {
                Renderer::SyncShape(engine, convex_shape, convex_comp, entity);
                convex_shape.setPointCount(convex_comp.vertices.size());
                for (std::size_t i = 0; i < convex_comp.vertices.size(); ++i)
                {
                    convex_shape.setPoint(i, cast::ToSfVec2(convex_comp.vertices.at(i)));
                }
            }
        }

        template <typename TComp>
        static void RenderCircle(Engine& engine, Component<TComp>& comp, const CircleShape& circle_comp, std::size_t index)
        {
            auto& window = engine.window.window_io;
            auto entity = comp.GetEntity();
            auto key = Window::DrawableKey(entity, Window::DrawableType::Circle, index);
            auto draw_itr = Renderer::EmplaceDrawables<sf::CircleShape>(window, key, comp);
            auto& circle_shape = static_cast<sf::CircleShape&>(*draw_itr->second);
            
            if (Renderer::InAnyViewHelper(window, engine, circle_comp, entity, key))
            {
                Renderer::SyncShape(engine, circle_shape, circle_comp, entity);
                circle_shape.setRadius(circle_comp.radius);
                circle_shape.setPointCount(circle_comp.vertex_count);
            }
        }

        template <typename TComp>
        static void RenderRectangle(Engine& engine, Component<TComp>& comp, const RectangleShape& rectangle_comp, std::size_t index)
        {
            auto& window = engine.window.window_io;
            auto entity = comp.GetEntity();
            auto key = Window::DrawableKey(entity, Window::DrawableType::Rectangle, index);
            auto draw_itr = Renderer::EmplaceDrawables<sf::RectangleShape>(window, key, comp);
            auto& rectangle_shape = static_cast<sf::RectangleShape&>(*draw_itr->second);

            if (Renderer::InAnyViewHelper(window, engine, rectangle_comp, entity, key))
            {
                Renderer::SyncShape(engine, rectangle_shape, rectangle_comp, entity);
                rectangle_shape.setSize(cast::ToSfVec2(rectangle_comp.size));
            }
        }

        template <typename TComp>
        static void RenderCustom(Engine& engine, Component<TComp>& comp, const CustomShape& custom_comp, std::size_t index)
        {
            auto& window = engine.window.window_io;
            auto entity = comp.GetEntity();
            auto key = Window::DrawableKey(entity, Window::DrawableType::Custom, index);
            auto draw_itr = Renderer::EmplaceDrawables<custom_sf::DrawableVertices>(window, key, comp);
            auto& vertices = static_cast<custom_sf::DrawableVertices&>(*draw_itr->second);

            if (Renderer::InAnyViewHelper(window, engine, custom_comp, entity, key))
            {
                auto size = custom_comp.vertices.size();

                vertices.type = cast::ToSfPolygonType(custom_comp.type);

                if (vertices.vertices.size() < custom_comp.vertices.size())
                {
                    vertices.vertices.resize(size);
                }

                for (std::size_t i = 0; i < size; ++i)
                {
                    auto& vertex = custom_comp.vertices.at(i);
                    vertices.vertices.at(i).position = 
                        cast::ToSfVec2(((vertex.position - custom_comp.origin) * global::GetScale(entity)).Rotated(global::GetRotation(entity))
                        + global::GetPosition(entity));
                    vertices.vertices.at(i).color = cast::ToSfColor(vertex.color);
                    vertices.vertices.at(i).texCoords = cast::ToSfVec2(vertex.texture_coordinates);
                }

                if (custom_comp.texture_id != "")
                {
                    vertices.texture = &engine.assets.GetTexture(custom_comp.texture_id).texture;
                }
                else
                {
                    vertices.texture = nullptr;
                }
            }
        }

        template <typename TComp>
        static void RenderSprite(Engine& engine, Component<TComp>& comp, const Sprite& sprite_comp, std::size_t index)
        {
            auto& window = engine.window.window_io;
            auto entity = comp.GetEntity();
            auto key = Window::DrawableKey(entity, Window::DrawableType::Sprite, index);
            auto draw_itr = Renderer::EmplaceDrawables<sf::Sprite>(window, key, comp);
            auto& sprite = static_cast<sf::Sprite&>(*draw_itr->second);

            if (Renderer::InAnyViewHelper(window, engine, sprite_comp, entity, key))
            {
                Renderer::SyncTransformable(sprite, sprite_comp, entity);

                auto& texture = engine.assets.GetTexture(sprite_comp.texture_id);

                Recti rect = sprite_comp.GetActualRectangle(engine.assets);

                auto scale = sprite_comp.size / rect.GetSize();
                sprite.setScale(sprite.getScale().x * scale.x, sprite.getScale().y * scale.y);
                sprite.setOrigin(sprite.getOrigin().x / scale.x, sprite.getOrigin().y / scale.y);

                sprite.setTexture(texture.texture);

                sprite.setTextureRect(cast::ToSfRect(rect));
                sprite.setColor(cast::ToSfColor(sprite_comp.color));
            }
        }

        template <typename TComp>
        static void RenderText(Engine& engine, Component<TComp>& comp, const doge::Text& text_comp, std::size_t index)
        {
            auto& window = engine.window.window_io;
            auto entity = comp.GetEntity();
            auto key = Window::DrawableKey(entity, Window::DrawableType::Text, index);
            auto draw_itr = Renderer::EmplaceDrawables<custom_sf::Text>(window, key, comp, engine.assets, text_comp);
            auto& text = static_cast<custom_sf::Text&>(*draw_itr->second);

            if (Renderer::InAnyViewHelper(window, engine, text, entity, key))
            {
                Renderer::SyncTransformable(text, text_comp, entity);
                text.Update(engine.assets, text_comp);
            }
        }

    private:

        template <typename TComp>
        static void SyncTransformable(sf::Transformable& transform, const TComp& comp, const Entity& entity)
        {
            transform.setOrigin(cast::ToSfVec2(comp.origin));
            transform.setScale(cast::ToSfVec2(global::GetScale(entity)));
            transform.setPosition(cast::ToSfVec2(global::GetPosition(entity)));
            transform.setRotation(cast::ToDegree(global::GetRotation(entity)));
        }

        template <typename TComp>
        static void SyncShape(const Engine& engine, sf::Shape& shape, const TComp& comp, const Entity& entity)
        {
            SyncTransformable(shape, comp, entity);
            shape.setFillColor(cast::ToSfColor(comp.color));
            shape.setOutlineColor(cast::ToSfColor(comp.outline_color));
            shape.setOutlineThickness(comp.outline_thickness);
            if (comp.texture_id != "")
            {
                auto& texture = engine.assets.GetTexture(comp.texture_id);
                shape.setTexture(&texture.texture);

                Recti rect = comp.texture_rectangle;
                if (!comp.atlas_rectangle_id.empty())
                {
                    rect = texture.atlas_rectangles.at(comp.atlas_rectangle_id);
                }

                shape.setTextureRect(cast::ToSfRect(rect));
            }
            else
            {
                shape.setTexture(nullptr);
            }
        }

        template <typename TDrawable, typename TComp, typename... Args>
        static auto EmplaceDrawables(Window& window, const Window::DrawableKey& key, Component<TComp>& comp, Args&&... args)
        {
            // layers_draws
            auto layer_comp = comp.GetEntity().template GetIfHasComponentElseDefault<Layer>();
            for (auto layer : layer_comp.layers)
            {
                window.layers_draws[layer].emplace(key);
            }

            // drawables
            auto draw_itr = window.drawables.find(key);
            if (draw_itr != window.drawables.end())
                return draw_itr;
            
            auto [draw_itr_, success] = window.drawables.emplace(key, static_cast<std::unique_ptr<sf::Drawable>>(std::make_unique<TDrawable>(std::forward<Args>(args)...)));
            if (!success)
            {
                throw std::invalid_argument("More than one shape is found in entity");
            }
            draw_itr = std::move(draw_itr_);
            comp.OnRemoval([&, val_key = key](){ window.drawables.erase(val_key); });
            return draw_itr;
        }

        static bool InAnyViewHelperHelper(Window& window, const Engine& engine, const Rectf& aabb_, const Entity& entity, const Window::DrawableKey& key)
        {
            auto is_in_any_view = false;
            Rectf aabb = aabb_;

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
                auto& [view_ptr, draw_keys] = window.views_draws.at(cam_entity.id);
                
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
        }

        template <typename TComp>
        static bool InAnyViewHelper(Window& window, const Engine& engine, const TComp& comp, const Entity& entity, const Window::DrawableKey& key)
        {
            if constexpr (std::is_same_v<TComp, custom_sf::Text>)
                return Renderer::InAnyViewHelperHelper(window, engine, comp.GetAABB(), entity, key);
            else if constexpr (std::is_same_v<TComp, Sprite>)
                return Renderer::InAnyViewHelperHelper(window, engine, global::GetAABB(comp, entity, engine), entity, key);
            else
                return Renderer::InAnyViewHelperHelper(window, engine, global::GetAABB(comp, entity), entity, key);
        }

        friend struct Window;
    };
}