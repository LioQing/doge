#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/core/Entity.hpp>
#include <doge/extensions/gui/Element.hpp>
#include <vector>
#include <string>

namespace doge::gui
{
    struct Window : Element
    {
        static const Vec2f DefaultSize;

        ~Window();
        void Initialize() override;

        std::int32_t GetLayer() const override;

        template <typename E>
        requires std::derived_from<std::remove_reference_t<E>, Element>
        E& AddElement(const std::string& id)
        {
            contained_element_ids.emplace(id);
            return GetGUI().AddElement<E>(id, GetWindowCameraID());
        }

        std::string GetWindowCameraID() const;

        void SetTextureID(const std::string& texture_id);
        std::string GetTextureID() const;

        void SetContainerBorderThickness(const Rectf& border_thickness);
        const Rectf& GetContainerBorderThickness() const;

        void SetCenterTextureSize(const Vec2i& center_texture_size);
        Vec2i GetCenterTextureSize() const;

        void SetBorderThickness(const Rectf& border_thickness);
        Rectf GetBorderThickness() const;

        void SetColor(const Color& color);
        const Color& GetColor() const;
    
    protected:

        void OnSizeUpdated() override;

    private:

        void UpdateContainerArea();

        Rectf border_thickness = Rectf(16, 16, 16, 16);

        std::unordered_set<std::string> contained_element_ids;

        Entity camera_entity;
    };
}