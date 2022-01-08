#include <doge/core/IOBus.hpp>

#include <memory>
#include <doge/core/Engine.hpp>
#include <doge/core/Texture.hpp>
#include <doge/components.hpp>
#include <doge/utils.hpp>

namespace doge
{
    // textures and images

    Vec2u IOBus::TextureData::GetSize() const
    {
        return cast::FromSfVec2(texture.getSize());
    }

    bool IOBus::TextureData::Create(const Vec2u& size)
    {
        return texture.create(size.x, size.y);
    }

    bool IOBus::TextureData::FromFile(const std::string& filename, const Recti& area)
    {
        return texture.loadFromFile(filename, cast::ToSfRect(area));
    }

    bool IOBus::TextureData::FromMemory(void* data, std::size_t size, const Recti& area)
    {
        return texture.loadFromMemory(data, size, cast::ToSfRect(area));
    }

    bool IOBus::TextureData::FromImage(const ImageData& image, const Recti& area)
    {
        return texture.loadFromImage(image.image, cast::ToSfRect(area));
    }

    void IOBus::TextureData::SetSmooth(bool smooth)
    {
        texture.setSmooth(smooth);
    }

    bool IOBus::TextureData::IsSmooth() const
    {
        return texture.isSmooth();
    }

    void IOBus::TextureData::SetSRGB(bool srgb)
    {
        texture.setSrgb(srgb);
    }

    bool IOBus::TextureData::IsSRGB() const
    {
        return texture.isSrgb();
    }

    void IOBus::TextureData::SetRepeated(bool repeated)
    {
        texture.setRepeated(repeated);
    }

    bool IOBus::TextureData::IsRepeated() const
    {
        return texture.isRepeated();
    }

    Vec2u IOBus::ImageData::GetSize() const
    {
        return cast::FromSfVec2(image.getSize());
    }

    void IOBus::ImageData::Create(const Vec2u& size, const Color& color)
    {
        image.create(size.x, size.y, cast::ToSfColor(color));
    }

    bool IOBus::ImageData::FromFile(const std::string& filename)
    {
        return image.loadFromFile(filename);
    }

    bool IOBus::ImageData::FromMemory(void* data, std::size_t size)
    {
        return image.loadFromMemory(data, size);
    }

    void IOBus::ImageData::FromTexture(const TextureData& texture)
    {
        image = texture.texture.copyToImage();
    }

    bool IOBus::ImageData::ToFile(const std::string& filename) const 
    {
        return image.saveToFile(filename);
    }

    void IOBus::ImageData::MaskColor(const Color& color, std::uint8_t alpha)
    {
        image.createMaskFromColor(cast::ToSfColor(color), alpha);
    }

    void IOBus::ImageData::SetPixel(std::uint32_t x, std::uint32_t y, const Color& color)
    {
        image.setPixel(x, y, cast::ToSfColor(color));
    }

    Color IOBus::ImageData::GetPixel(std::uint32_t x, std::uint32_t y) const
    {
        return cast::FromSfColor(image.getPixel(x, y));
    }

    const std::uint8_t* IOBus::ImageData::GetPixelPtr() const
    {
        return image.getPixelsPtr();
    }

    // window

    void IOBus::CreateWindow(const WindowSettings& window_settings)
    {
        if (window_settings.mode == WindowSettings::Mode::FullScreen)
        {
            window.create(sf::VideoMode(window_settings.resolution.x, window_settings.resolution.y), window_settings.title, sf::Style::Fullscreen);
        }
        else if (window_settings.mode == WindowSettings::Mode::Borderless)
        {
            window.create(sf::VideoMode(window_settings.resolution.x, window_settings.resolution.y), window_settings.title, sf::Style::None);
            window.setPosition(sf::Vector2i(0, 0));
            window.setSize(sf::Vector2u(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height));
        }
        else
        {
            window.create(sf::VideoMode(window_settings.resolution.x, window_settings.resolution.y), window_settings.title, window_settings.window_style);
        }

        window.setFramerateLimit(window_settings.fps);
    }

    void IOBus::CloseWindow()
    {
        window.close();
    }

    // game loops

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

        auto EmplaceDrawables = [&]<typename TSfShape, typename TComp>(const DrawableKey& key, Component<TComp>& comp)
        {
            auto draw_itr = drawables.find(key);
            if (draw_itr != drawables.end())
                return draw_itr;
            
            auto [draw_itr_, success] = drawables.emplace(key, static_cast<std::unique_ptr<sf::Drawable>>(std::make_unique<TSfShape>()));
            if (!success)
            {
                throw std::invalid_argument(std::string("More than one shape is found in Entity ") + std::to_string(std::get<0>(key)));
            }
            draw_itr = std::move(draw_itr_);
            comp.OnRemoval([&, val_key = key](){ drawables.erase(val_key); });
            return draw_itr;
        };

        auto InAnyViewHelper = [&]<typename TComp>(
            const TComp& comp, 
            const Entity& entity, 
            const DrawableKey& key
            )
        {
            auto is_in_any_view = false;
            Rectf aabb = global::GetAABB(comp, entity);

            for (auto cam_entity : engine.Select<Camera>().Entities())
            {
                auto& [view_ptr, draw_keys] = views_draws.at(cam_entity.id);
                
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
                view_itr = views_draws.emplace(entity.id, std::make_pair(std::make_unique<sf::View>(), std::set<DrawableKey>())).first;
                cam.OnRemoval([&, eid = entity.id](){ views_draws.erase(eid); });
            }

            auto& view = view_itr->second.first;
            
            view->setCenter(cast::ToSfVec2(global::GetPosition(entity)));
            if (cam.size == Vec2f::Zero())
            {
                view->setSize(cast::ToSfVec2(engine.window_settings.resolution * Vec2f(cam.port.width, cam.port.height) * global::GetScale(entity)));
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

        // compound shape
        for (auto [entity, compound_comp] : engine.Select<CompoundShape>().EntitiesAndComponents())
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
        }

        // draw
        window.clear();
        for (auto& [eid, view_draw] : views_draws)
        {
            auto& [view, draw_keys] = view_draw;

            window.setView(*view);

            // draw
            for (auto draw_key : draw_keys)
            {
                window.draw(*drawables.at(draw_key));
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