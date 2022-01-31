#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/core/Entity.hpp>
#include <doge/extensions/gui/Element.hpp>

namespace doge::gui
{
    struct Window : Element
    {
        static const Vec2f DefaultSize;

        ~Window();
        void Initialize() override;
    
    protected:

        void OnSizeUpdated() override;

    private:

        std::string texture_id = "doge_gui_button";
        Vec2i center_texture_size = Vec2i::Zero;
        Rectf border_thickness = Rectf();
        Color color = Color::White;

        Entity camera_entity;
    };
}