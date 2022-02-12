#pragma once

#include <doge/extensions/gui/Element.hpp>
#include <doge/utils/lev.hpp>
#include <doge/utils/Vec2.hpp>

namespace doge::gui
{
    struct Image;
    struct NSImage;
    struct Draggable;

    struct Slider : Element
    {
        enum Direction
        {
            Horizontal, Vertical,
        };

        Event<float> on_value_changed;

        virtual ~Slider();

        virtual void Initialize() override;

        void SetMinValue(float min_value);
        float GetMinValue() const;

        void SetMaxValue(float max_value);
        float GetMaxValue() const;

        void SetValue(float value);
        float GetValue() const;

        void SetInt(bool is_int);
        bool IsInt() const;

        void SetDirection(Direction direction);
        Direction GetDirection() const;

        std::string GetDraggableElementID() const;
        Draggable& GetDraggable() const;

        std::string GetTrackImageElementID() const;
        NSImage& GetTrackImage() const;

        std::string GetThumbImageElementID() const;
        Image& GetThumbImage() const;

        static const Vec2f DefaultSize;
        static const Vec2f DefaultTrackSize;

    protected:

        virtual void OnLayerUpdated() override;
        virtual void OnSizeUpdated() override;
        virtual void OnOriginUpdated() override;
        virtual void OnColorUpdated() override;

    private:

        bool is_int = false;
        Direction direction = Direction::Horizontal;
        float min_value = 0.f;
        float max_value = 100.f;
        float value = 0.f;
    };
}