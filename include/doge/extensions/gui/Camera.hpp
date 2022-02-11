#pragma once

#include <string>
#include <concepts>
#include <doge/components/Layer.hpp>
#include <doge/core/Entity.hpp>
#include <doge/utils/lev.hpp>
#include <doge/components/Camera.hpp>
#include <doge/core/Component.hpp>

namespace doge::gui
{
    struct GUI;

    struct Camera
    {
        Event<Vec2f, io::Mouse::Button> on_pressed;
        Event<Vec2f, io::Mouse::Button> on_released;
        Event<Vec2f, io::Mouse::Wheel, float> on_scrolled;
        Event<Vec2f> on_cursor_moved;
        Event<Vec2f> on_focus_gained;
        Event<Vec2f> on_focus_lost;

        GUI& GetGUI() const;
        Entity GetEntity() const;
        doge::Component<doge::Camera>& GetComponent() const;

        void SetLayer(const Layer& layer);
        bool HasLayer(std::int32_t layer) const;
        Layer& GetLayer() const;

        const std::string& GetID() const;

        void SetCameraPosition(const Vec2f& cam_pos);
        const Vec2f& GetCameraPosition() const;

        void SetPosition(const Vec2f& position);
        const Vec2f& GetPosition() const;

        void SetSize(const Vec2f& size);
        const Vec2f& GetSize() const;

        void SetOrigin(const Vec2f& origin);
        const Vec2f& GetOrigin() const;

        void SetAlign(std::uint8_t align);
        void SetAlign(const Vec2f& align);
        const Vec2f& GetAlign() const;

        void SetAbsolutePort(bool is_absolute_port);
        bool IsAbsolutePort() const;

        Vec2f GetActualOrigin() const;

    private:

        void Initialize();
        void UpdatePort();

        std::string id;
        bool is_absolute_port = false;
        Vec2f position = Vec2f::Zero;
        Vec2f origin = Vec2f::Zero;
        Vec2f align = Vec2f(.5f, .5f);

        GUI* gui = nullptr;

        friend struct GUI;
    };
}