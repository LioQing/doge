#pragma once

#include <doge/extensions/gui/CursorDetectableElement.hpp>
#include <doge/utils/Rect.hpp>
#include <string>

namespace doge::gui
{
    struct Scrollable : CursorDetectableElement
    {
        virtual ~Scrollable();

        virtual void Initialize() override;

        void SetScrollableCamera(const std::string& id);
        const std::string& GetScrollableCameraID() const;
        Camera& GetScrollableCamera() const;

        void SetArea(const Rectf& area);
        const Rectf& GetArea() const;

        void SetAreaUnlimited(bool unlimited);
        bool IsAreaUnlimited() const;

        void SetScrollSpeed(const Vec2f& scroll_speed);
        void SetScrollSpeed(float scroll_speed);
        const Vec2f& GetScrollSpeed() const;

    protected:

        virtual void OnSizeUpdated() override;

    private:

        void Bound();
        void OnCameraScrolled(const Vec2f& position, io::Mouse::Wheel wheel, float delta);

        std::string scrollable_camera;

        Rectf area = Rectf();
        bool area_unlimited = true;

        Vec2f scroll_speed = Vec2f(16, 16);
    };
}