#include <doge/extensions/gui/Resizable.hpp>

#include <doge/extensions/gui/Draggable.hpp>
#include <doge/extensions/gui/GUI.hpp>

namespace doge::gui
{
    Resizable::~Resizable()
    {
    }

    void Resizable::Initialize()
    {
        Draggable& tl   = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::TopLeft    ), GetCameraID());
        Draggable& t    = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::Top        ), GetCameraID());
        Draggable& tr   = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::TopRight   ), GetCameraID());
        Draggable& r    = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::Right      ), GetCameraID());
        Draggable& br   = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::BottomRight), GetCameraID());
        Draggable& b    = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::Bottom     ), GetCameraID());
        Draggable& bl   = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::BottomLeft ), GetCameraID());
        Draggable& l    = GetGUI().AddElement<Draggable>(GetDraggableElementID(Border::Left       ), GetCameraID());
        
        tl.GetEntity().SetParent(GetEntity());
        t .GetEntity().SetParent(GetEntity());
        tr.GetEntity().SetParent(GetEntity());
        r .GetEntity().SetParent(GetEntity());
        br.GetEntity().SetParent(GetEntity());
        b .GetEntity().SetParent(GetEntity());
        bl.GetEntity().SetParent(GetEntity());
        l .GetEntity().SetParent(GetEntity());
        
        tl.SetAlign(Align::Top      | Align::Left   );
        t .SetAlign(Align::Top      | Align::Center );
        tr.SetAlign(Align::Top      | Align::Right  );
        r .SetAlign(Align::Center   | Align::Right  );
        br.SetAlign(Align::Bottom   | Align::Right  );
        b .SetAlign(Align::Bottom   | Align::Center );
        bl.SetAlign(Align::Bottom   | Align::Left   );
        l .SetAlign(Align::Center   | Align::Left   );

        tl.on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::TopLeft;        start_size = GetSize(); on_resize_began(); };
        t .on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::Top;            start_size = GetSize(); on_resize_began(); };
        tr.on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::TopRight;       start_size = GetSize(); on_resize_began(); };
        r .on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::Right;          start_size = GetSize(); on_resize_began(); };
        br.on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::BottomRight;    start_size = GetSize(); on_resize_began(); };
        b .on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::Bottom;         start_size = GetSize(); on_resize_began(); };
        bl.on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::BottomLeft;     start_size = GetSize(); on_resize_began(); };
        l .on_drag_began += [&](const Vec2f& pos){ border_dragged = Border::Left;           start_size = GetSize(); on_resize_began(); };

        tl.on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        t .on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        tr.on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        r .on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        br.on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        b .on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        bl.on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };
        l .on_drag_ended += [&](const Vec2f& pos){ border_dragged = Border::Count; on_resize_ended(); };

        tl.on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f(-1, -1)); on_resized((final_size - start_size) * (Vec2f(-1, -1) + GetAlign() * Vec2f( 1,  1))); };
        t .on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f( 0, -1)); on_resized((final_size - start_size) * (Vec2f( 0, -1) + GetAlign() * Vec2f( 0,  1))); };
        tr.on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f( 1, -1)); on_resized((final_size - start_size) * (Vec2f( 0, -1) + GetAlign() * Vec2f( 1,  1))); };
        r .on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f( 1,  0)); on_resized((final_size - start_size) * (Vec2f( 0,  0) + GetAlign() * Vec2f( 1,  0))); };
        br.on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f( 1,  1)); on_resized((final_size - start_size) * (Vec2f( 0,  0) + GetAlign() * Vec2f( 1,  1))); };
        b .on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f( 0,  1)); on_resized((final_size - start_size) * (Vec2f( 0,  0) + GetAlign() * Vec2f( 0,  1))); };
        bl.on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f(-1,  1)); on_resized((final_size - start_size) * (Vec2f(-1,  0) + GetAlign() * Vec2f( 1,  1))); };
        l .on_dragged_diff += [&](const Vec2f& diff){ auto final_size = SetSizeChecked(start_size + diff * Vec2f(-1,  0)); on_resized((final_size - start_size) * (Vec2f(-1,  0) + GetAlign() * Vec2f( 1,  0))); };
    }

    void Resizable::SetThickness(const Rectf& thickness)
    {
        this->thickness = thickness;
    }

    const Rectf& Resizable::GetThickness() const
    {
        return thickness;
    }

    void Resizable::SetMinSize(const Vec2f& min_size)
    {
        this->min_size = min_size;
    }

    const Vec2f& Resizable::GetMinSize() const
    {
        return min_size;
    }

    void Resizable::SetLayer(std::int32_t layer)
    {
        for (auto i = 0; i < Border::Count; ++i)
            GetDraggable(i).SetLayer(layer);
    }

    std::int32_t Resizable::GetLayer() const
    {
        return GetDraggable(0).GetLayer();
    }

    std::string Resizable::GetDraggableElementID(std::int8_t border) const
    {
        switch (border)
        {
            case Border::TopLeft:       return "doge_gui_resizable_" + GetID() + "_draggable_topleft";
            case Border::Top:           return "doge_gui_resizable_" + GetID() + "_draggable_top";
            case Border::TopRight:      return "doge_gui_resizable_" + GetID() + "_draggable_topright";
            case Border::Right:         return "doge_gui_resizable_" + GetID() + "_draggable_right";
            case Border::BottomRight:   return "doge_gui_resizable_" + GetID() + "_draggable_bottomright";
            case Border::Bottom:        return "doge_gui_resizable_" + GetID() + "_draggable_bottom";
            case Border::BottomLeft:    return "doge_gui_resizable_" + GetID() + "_draggable_bottomleft";
            case Border::Left:          return "doge_gui_resizable_" + GetID() + "_draggable_left";
            default:                    throw std::invalid_argument("Invalid Resizable::Border");
        }
    }

    Draggable& Resizable::GetDraggable(std::int8_t border) const
    {
        return static_cast<Draggable&>(GetGUI().GetElement(GetDraggableElementID(border)));
    }

    void Resizable::OnSizeUpdated()
    {
        GetDraggable(Border::TopLeft    ).SetSize(Vec2f(GetThickness().left, GetThickness().top));
        GetDraggable(Border::Top        ).SetSize(Vec2f(GetSize().x - GetThickness().left - GetThickness().width, GetThickness().top));
        GetDraggable(Border::TopRight   ).SetSize(Vec2f(GetThickness().width, GetThickness().top));
        GetDraggable(Border::Right      ).SetSize(Vec2f(GetThickness().width, GetSize().y - GetThickness().top - GetThickness().height));
        GetDraggable(Border::BottomRight).SetSize(Vec2f(GetThickness().width, GetThickness().height));
        GetDraggable(Border::Bottom     ).SetSize(Vec2f(GetSize().x - GetThickness().left - GetThickness().width, GetThickness().height));
        GetDraggable(Border::BottomLeft ).SetSize(Vec2f(GetThickness().left, GetThickness().height));
        GetDraggable(Border::Left       ).SetSize(Vec2f(GetThickness().left, GetSize().y - GetThickness().top - GetThickness().height));
    }

    void Resizable::OnPositionUpdated()
    {
        for (auto i = 0; i < Border::Count; ++i)
            GetDraggable(i).SetPosition(GetPosition());
    }

    void Resizable::OnOriginUpdated()
    {
        GetDraggable(Border::TopLeft    ).SetOrigin(GetActualOrigin());
        GetDraggable(Border::Top        ).SetOrigin(GetActualOrigin() - Vec2f(GetSize().x / 2.f, 0.f));
        GetDraggable(Border::TopRight   ).SetOrigin(GetActualOrigin() - Vec2f(GetSize().x, 0.f));
        GetDraggable(Border::Right      ).SetOrigin(GetActualOrigin() - Vec2f(GetSize().x, GetSize().y / 2.f));
        GetDraggable(Border::BottomRight).SetOrigin(GetActualOrigin() - GetSize());
        GetDraggable(Border::Bottom     ).SetOrigin(GetActualOrigin() - Vec2f(GetSize().x / 2.f, GetSize().y));
        GetDraggable(Border::BottomLeft ).SetOrigin(GetActualOrigin() - Vec2f(0.f, GetSize().y));
        GetDraggable(Border::Left       ).SetOrigin(GetActualOrigin() - Vec2f(0.f, GetSize().y / 2.f));
    }

    Vec2f Resizable::SetSizeChecked(const Vec2f& size) 
    {
        auto act_size = size;

        if (act_size.x < min_size.x)
            act_size.x = min_size.x;

        if (act_size.y < min_size.y)
            act_size.y = min_size.y;
        
        SetSize(act_size);

        return act_size;
    }
}