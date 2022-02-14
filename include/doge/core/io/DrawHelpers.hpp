#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include <doge/core.hpp>
#include <doge/components.hpp>
#include <doge/utils.hpp>

namespace doge::io
{
    struct DrawHelpers
    {
    private:

        DrawHelpers(const DrawHelpers&) = delete;

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

        friend struct Window;
    };
}