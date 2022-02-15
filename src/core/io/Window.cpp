#include <doge/core/io/Window.hpp>

#include <memory>
#include <doge/core.hpp>
#include <doge/components.hpp>
#include <doge/utils.hpp>
#include <doge/core/io/Renderer.hpp>

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

    void Window::Render(Engine& engine)
    {
        layers_draws.clear();

        // view
        for (auto [entity, cam] : engine.Select<Camera>().EntitiesAndComponents())
        {
            auto view_itr = views_draws.find(entity.id);
            if (view_itr == views_draws.end())
            {
                view_itr = views_draws.emplace(entity.id, ViewInfo(std::make_unique<sf::View>(), std::set<DrawableKey>())).first;
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
        for (auto [convex_comp] : engine.Select<ConvexShape>().Components())
        {
            Renderer::RenderConvexShape(engine, convex_comp, convex_comp, 0);
        }

        // circle shape
        for (auto [entity, circle_comp] : engine.Select<CircleShape>().EntitiesAndComponents())
        {
            Renderer::RenderCircle(engine, circle_comp, circle_comp, 0);
        }

        // rectangle shape
        for (auto [entity, rectangle_comp] : engine.Select<RectangleShape>().EntitiesAndComponents())
        {
            Renderer::RenderRectangle(engine, rectangle_comp, rectangle_comp, 0);
        }

        // custom shape
        for (auto [entity, custom_comp] : engine.Select<CustomShape>().EntitiesAndComponents())
        {
            Renderer::RenderCustom(engine, custom_comp, custom_comp, 0);
        }

        // sprite
        for (auto [entity, sprite_comp] : engine.Select<doge::Sprite>().EntitiesAndComponents())
        {
            Renderer::RenderSprite(engine, sprite_comp, sprite_comp, 0);
        }

        // text
        for (auto [entity, text_comp] : engine.Select<doge::Text>().EntitiesAndComponents())
        {
            Renderer::RenderText(engine, text_comp, text_comp, 0);
        }

        // compound shape
        for (auto [entity, compound_comp] : engine.Select<CompoundSprite>().EntitiesAndComponents())
        {
            // convex sub shape
            for (std::size_t i = 0; i < compound_comp.convex_shapes.size(); ++i)
            {
                Renderer::RenderConvexShape(engine, compound_comp, compound_comp.convex_shapes.at(i), i);
            }

            // circle sub shape
            for (std::size_t i = 0; i < compound_comp.circle_shapes.size(); ++i)
            {
                Renderer::RenderCircle(engine, compound_comp, compound_comp.circle_shapes.at(i), i);
            }

            // rectangle sub shape
            for (std::size_t i = 0; i < compound_comp.rectangle_shapes.size(); ++i)
            {
                Renderer::RenderRectangle(engine, compound_comp, compound_comp.rectangle_shapes.at(i), i);
            }

            // custom sub shape
            for (std::size_t i = 0; i < compound_comp.custom_shapes.size(); ++i)
            {
                Renderer::RenderCustom(engine, compound_comp, compound_comp.custom_shapes.at(i), i);
            }

            // sub sprites
            for (std::size_t i = 0; i < compound_comp.sprites.size(); ++i)
            {
                Renderer::RenderSprite(engine, compound_comp, compound_comp.sprites.at(i), i);
            }

            // text sub sprites
            for (std::size_t i = 0; i < compound_comp.texts.size(); ++i)
            {
                Renderer::RenderText(engine, compound_comp, compound_comp.texts.at(i), i);
            }
        }
    }

    void Window::Draw(Engine& engine)
    {
        window.clear(cast::ToSfColor(background_color));

        auto cdf_itr = custom_draw_functions.begin();
        for (auto& [layer, draw_keys] : layers_draws)
        {
            for (auto& draw_key : draw_keys)
            {
                for (auto& [cam_id, view_info] : views_draws)
                {
                    auto& [view, view_draws] = view_info;

                    if (
                        view_draws.contains(draw_key) &&
                        (!engine.GetEntity(cam_id).HasComponent<Layer>() && layer == 0 ||
                        engine.GetEntity(cam_id).HasComponent<Layer>() &&
                        engine.GetEntity(cam_id).GetComponent<Layer>().layers.contains(layer))
                    )
                    {
                        window.setView(*view);
                        window.draw(*drawables.at(draw_key));
                    }
                }
            }

            for (; cdf_itr != custom_draw_functions.end() && cdf_itr->first <= layer; ++cdf_itr)
            {
                for (auto& [id, cdf] : cdf_itr->second)
                {
                    cdf(engine);
                }
            }
        }

        for (; cdf_itr != custom_draw_functions.end(); ++cdf_itr)
        {
            for (auto& [id, cdf] : cdf_itr->second)
            {
                cdf(engine);
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

    void Window::RemoveDrawFunction(std::int32_t layer, const std::string& id)
    {
        custom_draw_functions.at(layer).erase(id);

        if (custom_draw_functions.at(layer).empty())
            custom_draw_functions.erase(layer);
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