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

    void IOBus::Render(const Engine& e)
    {
        std::vector<std::tuple<sf::View, std::vector<sf::CircleShape>, std::vector<sf::ConvexShape>, std::vector<sf::RectangleShape>>> views_shapes;
        for (auto [entity, camera] : e.Select<Camera>().EntitiesAndComponents())
        {
            // view
            sf::View view(cast::ToSfVec2(global::GetPosition(camera)), cast::ToSfVec2(camera.size * global::GetScale(camera)));
            if (camera.size == Vec2f::Zero())
            {
                view.setSize(cast::ToSfVec2(e.GetVideoSettings().resolution * Vec2f(camera.port.width, camera.port.height) * global::GetScale(camera)));
            }
            view.setRotation(cast::ToDegree(global::GetRotation(camera)));
            view.setViewport(cast::ToSfRect(camera.port));

            // circles
            std::vector<sf::CircleShape> circle_shapes;

            for (auto [entity, circle_comp] : e.Select<CircleShape>().EntitiesAndComponents())
            {
                auto circle_shape = sf::CircleShape(circle_comp.radius);
                circle_shape.setOrigin(cast::ToSfVec2(circle_comp.origin));
                circle_shape.setScale(cast::ToSfVec2(global::GetScale(circle_comp)));
                circle_shape.setPosition(cast::ToSfVec2(global::GetPosition(circle_comp)));
                circle_shape.setFillColor(cast::ToSfColor(circle_comp.color));
                circle_shape.setPointCount(circle_comp.point_count);
                circle_shapes.emplace_back(circle_shape);
            }

            // convex shapes
            std::vector<sf::ConvexShape> convex_shapes;

            for (auto [entity, convex_comp] : e.Select<ConvexShape>().EntitiesAndComponents())
            {
                auto convex_shape = sf::ConvexShape(convex_comp.points.size());
                for (std::size_t i = 0; i < convex_comp.points.size(); ++i)
                {
                    convex_shape.setPoint(i, cast::ToSfVec2(convex_comp.points.at(i)));
                }
                convex_shape.setOrigin(cast::ToSfVec2(convex_comp.origin));
                convex_shape.setScale(cast::ToSfVec2(global::GetScale(convex_comp)));
                convex_shape.setPosition(cast::ToSfVec2(global::GetPosition(convex_comp)));
                convex_shape.setRotation(cast::ToDegree(global::GetRotation(convex_comp)));
                convex_shape.setFillColor(cast::ToSfColor(convex_comp.color));
                convex_shapes.emplace_back(convex_shape);
            }

            // rectangle shapes
            std::vector<sf::RectangleShape> rectangle_shapes;

            for (auto [entity, rectangle_comp] : e.Select<RectangleShape>().EntitiesAndComponents())
            {
                auto rectangle_shape = sf::RectangleShape(cast::ToSfVec2(rectangle_comp.size));
                rectangle_shape.setOrigin(cast::ToSfVec2(rectangle_comp.origin));
                rectangle_shape.setScale(cast::ToSfVec2(global::GetScale(rectangle_comp)));
                rectangle_shape.setPosition(cast::ToSfVec2(global::GetPosition(rectangle_comp)));
                rectangle_shape.setRotation(cast::ToDegree(global::GetRotation(rectangle_comp)));
                rectangle_shape.setFillColor(cast::ToSfColor(rectangle_comp.color));
                rectangle_shapes.emplace_back(rectangle_shape);
            }

            // add to views shapes
            views_shapes.emplace_back(view, circle_shapes, convex_shapes, rectangle_shapes);
        }

        window.clear();
        for (auto [view, circle_shapes, convex_shapes, rectangle_shapes] : views_shapes)
        {
            window.setView(view);

            // draw
            for (auto drawable : circle_shapes)
            {
                window.draw(drawable);
            }
            for (auto drawable : convex_shapes)
            {
                window.draw(drawable);
            }
            for (auto drawable : rectangle_shapes)
            {
                window.draw(drawable);
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