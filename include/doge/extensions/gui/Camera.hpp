#pragma once

#include <string>
#include <concepts>
#include <doge/components/Layer.hpp>
#include <doge/core/Entity.hpp>

namespace doge::gui
{
    struct GUI;

    struct Camera
    {

        GUI& GetGUI() const;
        Entity GetEntity() const;

        void SetLayer(const Layer& layer);
        bool HasLayer(std::int32_t layer) const;
        Layer& GetLayer() const;

        const std::string& GetID() const;

        void SetPosition(const Vec2f& position);
        const Vec2f& GetPosition() const;

        void SetSize(const Vec2f& size);
        const Vec2f& GetSize() const;

        void SetAbsolutePort(bool is_absolute_port);
        bool IsAbsolutePort() const;

    private:

        void Initialize();

        std::string id;
        bool is_absolute_port = false;

        GUI* gui = nullptr;

        friend struct GUI;
    };
}