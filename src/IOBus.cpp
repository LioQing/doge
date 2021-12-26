#include <doge/core/IOBus.hpp>

#include <memory>
#include <doge/core/Engine.hpp>
#include <doge/components.hpp>
#include <doge/utils.hpp>

namespace doge
{
    void IOBus::CreateWindow(const VideoSettings& video_settings, const std::string& title)
    {
        if (video_settings.mode == VideoSettings::Mode::FullScreen)
        {
            window.create(sf::VideoMode(video_settings.resolution.x, video_settings.resolution.y), title, sf::Style::Fullscreen);
        }
        else if (video_settings.mode == VideoSettings::Mode::Borderless)
        {
            window.create(sf::VideoMode(video_settings.resolution.x, video_settings.resolution.y), title, sf::Style::None);
            window.setPosition(sf::Vector2i(0, 0));
            window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
        }
        else
        {
            window.create(sf::VideoMode(video_settings.resolution.x, video_settings.resolution.y), title, video_settings.window_style);
        }
    }

    void IOBus::CloseWindow()
    {
        window.close();
    }

    void IOBus::Render(const Engine& engine)
    {
        // helper functions
        auto SyncShape = []<typename TComp>(sf::Shape& shape, const TComp& comp, const Entity& entity)
        {
            shape.setOrigin(cast::ToSfVec2(comp.origin));
            shape.setScale(cast::ToSfVec2(global::GetScale(entity)));
            shape.setPosition(cast::ToSfVec2(global::GetPosition(entity)));
            shape.setRotation(cast::ToDegree(global::GetRotation(entity)));
            shape.setFillColor(cast::ToSfColor(comp.color));
        };

        auto EmplaceDrawables = [&]<typename TSfShape, typename TComp>(EntityID eid, Component<TComp>& comp)
        {
            auto draw_itr = drawables.find(eid);
            if (draw_itr != drawables.end())
                return draw_itr;
            
            auto [draw_itr_, success] = drawables.emplace(eid, static_cast<std::unique_ptr<sf::Drawable>>(std::make_unique<TSfShape>()));
            if (!success)
            {
                throw std::invalid_argument(std::string("More than one shape is found in Entity ") + std::to_string(eid));
            }
            draw_itr = std::move(draw_itr_);
            comp.OnRemoval([&, eid](){ drawables.erase(eid); });
            return draw_itr;
        };

        auto InAnyViewHelper = [&]<typename TComp>(
            const TComp& comp, 
            const Entity& entity, 
            std::uint8_t compound_arr_index = 0,
            std::size_t compound_vec_index = 0
            )
        {
            auto is_in_any_view = false;

            for (auto cam_entity : engine.Select<Camera>().Entities())
            {
                auto& [view_ptr, draw_ids] = views_draws.at(cam_entity.id);

                Rectf aabb = global::GetAABB(comp, entity);
                bool is_in_view = 
                    aabb.left + aabb.width > view_ptr->getCenter().x - view_ptr->getSize().x / 2.f &&
                    aabb.top + aabb.height > view_ptr->getCenter().y - view_ptr->getSize().y / 2.f &&
                    aabb.left < view_ptr->getCenter().x + view_ptr->getSize().x / 2.f &&
                    aabb.top < view_ptr->getCenter().y + view_ptr->getSize().y / 2.f;

                is_in_any_view = is_in_any_view || is_in_view;

                auto draw_id_itr = draw_ids.find(std::make_tuple(entity.id, compound_arr_index, compound_vec_index));
                if (draw_id_itr != draw_ids.end() && !is_in_view)
                {
                    draw_ids.erase(std::make_tuple(entity.id, compound_arr_index, compound_vec_index));
                }
                else if (draw_id_itr == draw_ids.end() && is_in_view)
                {
                    draw_ids.emplace(std::make_tuple(entity.id, compound_arr_index, compound_vec_index));
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
                view_itr = views_draws.emplace(entity.id, std::make_pair(std::make_unique<sf::View>(), std::set<std::tuple<EntityID, std::uint8_t, std::size_t>>())).first;
                cam.OnRemoval([&, eid = entity.id](){ views_draws.erase(eid); });
            }

            auto& view = view_itr->second.first;
            
            view->setCenter(cast::ToSfVec2(global::GetPosition(entity)));
            if (cam.size == Vec2f::Zero())
            {
                view->setSize(cast::ToSfVec2(engine.GetVideoSettings().resolution * Vec2f(cam.port.width, cam.port.height) * global::GetScale(entity)));
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
            view_draw.second.clear();
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

        for (auto [entity, convex_comp] : engine.Select<ConvexShape>().EntitiesAndComponents())
        {
            auto draw_itr = EmplaceDrawables.template operator()<sf::ConvexShape>(entity, convex_comp);

            if (InAnyViewHelper(convex_comp, entity))
            {
                SyncConvex(static_cast<sf::ConvexShape&>(*std::get<std::unique_ptr<sf::Drawable>>(draw_itr->second)), convex_comp, entity);
            }
        }

        // circle shape
        auto SyncCircle = [&](sf::CircleShape& circle_shape, const CircleShape& circle_comp, const Entity& entity)
        {
            SyncShape(circle_shape, circle_comp, entity);
            circle_shape.setRadius(circle_comp.radius);
            circle_shape.setPointCount(circle_comp.point_count);
        };

        for (auto [entity, circle_comp] : engine.Select<CircleShape>().EntitiesAndComponents())
        {
            auto draw_itr = EmplaceDrawables.template operator()<sf::CircleShape>(entity, circle_comp);
            
            if (InAnyViewHelper(circle_comp, entity))
            {
                SyncCircle(static_cast<sf::CircleShape&>(*std::get<std::unique_ptr<sf::Drawable>>(draw_itr->second)), circle_comp, entity);
            }
        }

        // rectangle shape
        auto SyncRectangle = [&](sf::RectangleShape& rectangle_shape, const RectangleShape& rectangle_comp, const Entity& entity)
        {
            SyncShape(rectangle_shape, rectangle_comp, entity);
            rectangle_shape.setSize(cast::ToSfVec2(rectangle_comp.size));
        };

        for (auto [entity, rectangle_comp] : engine.Select<RectangleShape>().EntitiesAndComponents())
        {
            auto draw_itr = EmplaceDrawables.template operator()<sf::RectangleShape>(entity, rectangle_comp);

            if (InAnyViewHelper(rectangle_comp, entity))
            {
                SyncRectangle(static_cast<sf::RectangleShape&>(*std::get<std::unique_ptr<sf::Drawable>>(draw_itr->second)), rectangle_comp, entity);
            }
        }

        // compound shape
        for (auto [entity, compound_comp] : engine.Select<CompoundShape>().EntitiesAndComponents())
        {
            auto draw_itr = drawables.find(entity.id);
            if (draw_itr == drawables.end())
            {
                auto [draw_itr_, success] = drawables.emplace(entity.id, std::array<std::vector<std::unique_ptr<sf::Drawable>>, 3>());
                if (!success)
                {
                    throw std::invalid_argument(std::string("More than one shape is found in Entity ") + std::to_string(entity.id));
                }
                draw_itr = std::move(draw_itr_);
                compound_comp.OnRemoval([&, eid = entity.id](){ drawables.erase(eid); });
            }

            auto& draw_arr = std::get<std::array<std::vector<std::unique_ptr<sf::Drawable>>, 3>>(draw_itr->second);

            // convex sub shape
            draw_arr.at(0).resize(compound_comp.convex_shapes.size());
            for (std::size_t i = 0; i < compound_comp.convex_shapes.size(); ++i)
            {
                if (!draw_arr.at(0).at(i))
                {
                    draw_arr.at(0).at(i) = std::make_unique<sf::ConvexShape>();
                }

                if (InAnyViewHelper(compound_comp.convex_shapes.at(i), entity, 0, i))
                {
                    SyncConvex(static_cast<sf::ConvexShape&>(*draw_arr.at(0).at(i)), compound_comp.convex_shapes.at(i), entity);
                }
            }

            // circle sub shape
            draw_arr.at(1).resize(compound_comp.circle_shapes.size());
            for (std::size_t i = 0; i < compound_comp.circle_shapes.size(); ++i)
            {
                if (!draw_arr.at(1).at(i))
                {
                    draw_arr.at(1).at(i) = std::make_unique<sf::CircleShape>();
                }
                
                if (InAnyViewHelper(compound_comp.circle_shapes.at(i), entity, 1, i))
                {
                    SyncCircle(static_cast<sf::CircleShape&>(*draw_arr.at(1).at(i)), compound_comp.circle_shapes.at(i), entity);
                }
            }

            // rectangle sub shape
            draw_arr.at(2).resize(compound_comp.rectangle_shapes.size());
            for (std::size_t i = 0; i < compound_comp.rectangle_shapes.size(); ++i)
            {
                if (!draw_arr.at(2).at(i))
                {
                    draw_arr.at(2).at(i) = std::make_unique<sf::RectangleShape>();
                }

                if (InAnyViewHelper(compound_comp.rectangle_shapes.at(i), entity, 2, i))
                {
                    SyncRectangle(static_cast<sf::RectangleShape&>(*draw_arr.at(2).at(i)), compound_comp.rectangle_shapes.at(i), entity);
                }
            }
        }

        // draw
        window.clear();
        for (auto& [eid, view_draw] : views_draws)
        {
            auto& [view, draw_ids] = view_draw;

            window.setView(*view);

            // draw
            for (auto draw_id : draw_ids)
            {
                auto eid = std::get<0>(draw_id);
                if (drawables.at(eid).index() == 0)
                {
                    window.draw(*std::get<std::unique_ptr<sf::Drawable>>(drawables.at(eid)));
                }
                else
                {
                    window.draw(*std::get<std::array<std::vector<std::unique_ptr<sf::Drawable>>, 3>>(drawables.at(eid))
                        .at(std::get<1>(draw_id))
                        .at(std::get<2>(draw_id))
                    );
                }
            }
        }
    }

    void IOBus::Display()
    {
        window.display();
    }

    void IOBus::SetFrameRate(uint32_t frame_rate)
    {
        window.setFramerateLimit(frame_rate);
    }

    void IOBus::StartDeltaClock()
    {
        clock.restart();
    }

    float IOBus::GetDeltaTime()
    {
        return clock.restart().asMicroseconds() / 1000.f;
    }
}