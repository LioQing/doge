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
        auto SyncShape = []<typename TComp>(sf::Shape& shape, const Component<TComp>& comp)
        {
            auto entity = comp.GetEntity();
            shape.setOrigin(cast::ToSfVec2(comp.origin));
            shape.setScale(cast::ToSfVec2(global::GetScale(entity)));
            shape.setPosition(cast::ToSfVec2(global::GetPosition(entity)));
            shape.setRotation(cast::ToDegree(global::GetRotation(entity)));
            shape.setFillColor(cast::ToSfColor(comp.color));
        };

        // view
        for (auto [entity, cam] : engine.Select<Camera>().EntitiesAndComponents())
        {
            auto view_itr = views_draws.find(entity.id);
            if (view_itr == views_draws.end())
            {
                view_itr = views_draws.emplace(entity.id, std::make_pair(std::make_unique<sf::View>(), std::vector<EntityID>())).first;
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

        // circle shape
        for (auto [entity, circle_comp] : engine.Select<CircleShape>().EntitiesAndComponents())
        {
            auto draw_itr = drawables.find(entity.id);
            if (draw_itr == drawables.end())
            {
                draw_itr = drawables.emplace(entity.id, std::make_unique<sf::CircleShape>()).first;
                circle_comp.OnRemoval([&, eid = entity.id](){ drawables.erase(eid); });
            }

            auto& circle_shape = static_cast<sf::CircleShape&>(*draw_itr->second);
            SyncShape(circle_shape, circle_comp);
            circle_shape.setRadius(circle_comp.radius);
            circle_shape.setPointCount(circle_comp.point_count);
        }

        // convex shape
        for (auto [entity, convex_comp] : engine.Select<ConvexShape>().EntitiesAndComponents())
        {
            auto draw_itr = drawables.find(entity.id);
            if (draw_itr == drawables.end())
            {
                draw_itr = drawables.emplace(entity.id, std::make_unique<sf::ConvexShape>()).first;
                convex_comp.OnRemoval([&, eid = entity.id](){ drawables.erase(eid); });
            }

            auto& convex_shape = static_cast<sf::ConvexShape&>(*draw_itr->second);
            SyncShape(convex_shape, convex_comp);
            convex_shape.setPointCount(convex_comp.points.size());
            for (std::size_t i = 0; i < convex_comp.points.size(); ++i)
            {
                convex_shape.setPoint(i, cast::ToSfVec2(convex_comp.points.at(i)));
            }
        }

        // rectangle shape
        for (auto [entity, rectangle_comp] : engine.Select<RectangleShape>().EntitiesAndComponents())
        {
            auto draw_itr = drawables.find(entity.id);
            if (draw_itr == drawables.end())
            {
                draw_itr = drawables.emplace(entity.id, std::make_unique<sf::RectangleShape>()).first;
                rectangle_comp.OnRemoval([&, eid = entity.id](){ drawables.erase(eid); });
            }

            auto& rectangle_shape = static_cast<sf::RectangleShape&>(*draw_itr->second);
            SyncShape(rectangle_shape, rectangle_comp);
            rectangle_shape.setSize(cast::ToSfVec2(rectangle_comp.size));
        }

        // draw
        window.clear();
        for (auto& [eid, view_draw] : views_draws)
        {
            auto& [view, draws] = view_draw;

            window.setView(*view);

            // draw
            for (auto& [eid, drawable] : drawables)
            {
                window.draw(*drawable);
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