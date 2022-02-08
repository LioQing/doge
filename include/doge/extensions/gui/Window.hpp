#pragma once

#include <doge/utils/Vec2.hpp>
#include <doge/utils/Rect.hpp>
#include <doge/core/Entity.hpp>
#include <doge/extensions/gui/Element.hpp>
#include <doge/extensions/gui/GUI.hpp>
#include <vector>
#include <string>

namespace doge::gui
{
    struct NSImage;

    struct Window : Element
    {
        virtual ~Window();

        virtual void Initialize() override;

        template <std::derived_from<Element> E>
        E& AddElement(const std::string& id)
        {
            contained_element_ids.emplace(id);
            return GetGUI().AddElement<E>(id, GetWindowCameraID());
        }

        std::string GetWindowCameraID() const;
        Camera& GetWindowCamera() const;

        void SetBorderThickness(const Rectf& border_thickness);
        const Rectf& GetBorderThickness() const;

        std::string GetImageElementID() const;
        NSImage& GetImage() const;
        
        static const Vec2f DefaultSize;

    protected:

        virtual void OnLayerUpdated() override;
        virtual void OnSizeUpdated() override;
        virtual void OnPositionUpdated() override;
        virtual void OnOriginUpdated() override;

    private:

        Rectf border_thickness = Rectf();

        std::unordered_set<std::string> contained_element_ids;
    };
}