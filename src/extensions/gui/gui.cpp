#include <doge/extensions/gui/GUI.hpp>

#include <doge/core/Engine.hpp>
#include <doge/extensions/nine_slice.hpp>
#include <doge/extensions/gui/CameraComponent.hpp>
#include <doge/extensions/gui/Camera.hpp>
#include <doge/components/Camera.hpp>
#include <algorithm>

namespace doge::gui
{
    GUI::GUI(Engine& engine) : engine(engine), nine_slice(engine)
    {
        engine.scenes.extensions.emplace("doge_gui", GameLoopFunctions::Create(*this, &GUI::Start, &GUI::Update, &GUI::FixedUpdate, &GUI::Finish));

        if (engine.scenes.is_running)
        {
            Start(engine);
        }
    }

    GUI::~GUI()
    {
        cameras.clear();
        elements.clear();

        engine.scenes.extensions.erase("doge_gui");
    }

    Camera& GUI::AddCamera(const std::string& id)
    {
        auto entity = engine.AddEntity();

        auto& comp = entity.AddComponent<CameraComponent>();
        comp.camera.id = id;
        comp.camera.gui = this;

        comp.OnRemoval([&, val_id = id]()
        {
            cameras.erase(val_id);
        });

        cameras.emplace(comp.camera.GetID(), entity);
        comp.camera.Initialize();

        return comp.camera;
    }

    void GUI::RemoveCamera(const std::string& id)
    {
        GetEngine().DestroyEntity(GetCameraEntity(id));
    }

    Camera& GUI::GetCamera(const std::string& id) const
    {
        return cameras.at(id).GetComponent<CameraComponent>().camera;
    }

    doge::Component<doge::Camera>& GUI::GetCameraComponent(const std::string& id) const
    {
        return cameras.at(id).GetComponent<doge::Camera>();
    }

    Entity GUI::GetCameraEntity(const std::string& id) const
    {
        return cameras.at(id);
    }

    bool GUI::HasCamera(const std::string& id) const
    {
        return cameras.find(id) != cameras.end();
    }

    void GUI::RemoveElement(const std::string& id)
    {
        engine.DestroyEntity(elements.at(id));
    }

    void GUI::RemoveElements(const std::string& camera_id)
    {
        std::vector<EntityID> to_be_destroyed;
        for (auto& [id, element] : elements)
        {
            if (element.GetComponent<ElementComponent>().element->GetCameraID() == camera_id)
                to_be_destroyed.emplace_back(element);
        }

        for (auto& element : to_be_destroyed)
        {
            engine.DestroyEntity(element);
        }
    }

    Element& GUI::GetElement(const std::string& id) const
    {
        return *GetElementComponent(id).element;
    }

    doge::Component<ElementComponent>& GUI::GetElementComponent(const std::string& id) const
    {
        return elements.at(id).GetComponent<ElementComponent>();
    }

    Entity GUI::GetElementEntity(const std::string& id) const
    {
        return elements.at(id);
    }

    bool GUI::HasElement(const std::string& id) const
    {
        return elements.find(id) != elements.end();
    }

    Engine& GUI::GetEngine() const
    {
        return engine;
    }

    doge::nine_slice::NineSlice& GUI::GetNineSlice()
    {
        return nine_slice;
    }

    const doge::nine_slice::NineSlice& GUI::GetNineSlice() const
    {
        return nine_slice;
    }

    std::shared_ptr<CursorDetectableElement> GUI::GetElementBelowCursor() const
    {
        return element_below_cursor;
    }

    void GUI::SetElementBelowCursorLocked(bool locked)
    {
        element_below_cursor_locked = locked;
    }

    bool GUI::IsElementBelowCursorLocked() const
    {
        return element_below_cursor_locked;
    }

    void GUI::Start(Engine& engine)
    {
        // default textures

        engine.assets.LoadTexture("doge_gui_button", "gui/button.png");
        nine_slice.LoadTexture("doge_gui_button", "gui/button.png", Recti(8, 8, 8, 8));

        engine.assets.LoadTexture("doge_gui_window", "gui/window.png");
        nine_slice.LoadTexture("doge_gui_window", "gui/window.png", Recti(12, 12, 12, 12));

        engine.assets.LoadTexture("doge_gui_windowex", "gui/windowex.png");
        nine_slice.LoadTexture("doge_gui_windowex", "gui/windowex.png", Recti(12, 32, 12, 12));

        {
            auto [itr, success] = engine.assets.LoadTexture("doge_gui_windowex_close_button", "gui/windowex_close_button.png");
            itr->second.atlas_rectangles.emplace("default", Recti(0, 0, 24, 24));
            itr->second.atlas_rectangles.emplace("pressed", Recti(24, 0, 24, 24));
        }

        // events

        engine.events.on_mouse_moved.AddListener("doge_gui",
        [&](const event::MouseMove& event)
        {
            if (cursor_event_called.test(0))
                return;
            cursor_event_called.set(0, true);

            // moved
            if (element_below_cursor)
            {
                auto cursor_pos = GetEngine().window.MapPixelToCoords(
                    event.position,
                    element_below_cursor->GetCameraComponent()
                );

                element_below_cursor->OnCursorMoved(cursor_pos);
                element_below_cursor->on_cursor_moved(cursor_pos);
            }

            if (IsElementBelowCursorLocked())
                return;

            // entered / left
            auto was = element_below_cursor;
            element_below_cursor = nullptr;

            for (auto& ptr : cursor_detectable_elements)
            {
                Vec2f cursor_pos = GetEngine().window.MapPixelToCoords(event.position, ptr->GetCameraComponent());
                Recti cam_rect = Recti(
                    ptr->GetCameraComponent().port.GetPosition() * GetEngine().window.window_io.GetSize(),
                    ptr->GetCameraComponent().port.GetSize() * GetEngine().window.window_io.GetSize()
                );

                if (
                    math::TestPoint(event.position, cam_rect) &&
                    ptr->TestPoint(cursor_pos) &&
                    (!element_below_cursor || ptr->GetLayer() > element_below_cursor->GetLayer())
                )
                {
                    element_below_cursor = ptr;
                }
            }

            if (element_below_cursor != was)
            {
                if (was)
                {
                    auto cursor_pos = GetEngine().window.MapPixelToCoords(
                        event.position,
                        was->GetCameraComponent()
                    );

                    was->OnCursorLeft(cursor_pos);
                    was->on_cursor_left(cursor_pos);
                }
                
                if (element_below_cursor)
                {
                    auto cursor_pos = GetEngine().window.MapPixelToCoords(
                        event.position,
                        element_below_cursor->GetCameraComponent()
                    );

                    element_below_cursor->OnCursorEntered(cursor_pos);
                    element_below_cursor->on_cursor_entered(cursor_pos);
                }
            }
        });

        engine.events.on_mouse_button_pressed.AddListener("doge_gui",
        [&](event::MouseButton event)
        {
            if (cursor_event_called.test(1))
                return;
            cursor_event_called.set(1, true);

            if (element_below_cursor)
            {
                auto cursor_pos = GetEngine().window.MapPixelToCoords(
                    event.position,
                    element_below_cursor->GetCameraComponent()
                );

                element_below_cursor->OnPressed(cursor_pos, event.button);
                element_below_cursor->on_pressed(cursor_pos, event.button);
            }
        });

        engine.events.on_mouse_button_released.AddListener("doge_gui",
        [&](event::MouseButton event)
        {
            if (cursor_event_called.test(2))
                return;
            cursor_event_called.set(2, true);

            if (element_below_cursor)
            {
                auto cursor_pos = GetEngine().window.MapPixelToCoords(
                    event.position,
                    element_below_cursor->GetCameraComponent()
                );

                element_below_cursor->OnReleased(cursor_pos, event.button);
                element_below_cursor->on_released(cursor_pos, event.button);
            }
        });
    }

    void GUI::Update(Engine& engine, DeltaTime dt)
    {
        cursor_event_called.reset();
    }

    void GUI::FixedUpdate(Engine& engine, DeltaTime dt)
    {
    }

    void GUI::Finish(Engine& engine)
    {
        cameras.clear();
        elements.clear();

        engine.assets.EraseTexture("doge_gui_button");
        nine_slice.EraseTexture("doge_gui_button");

        engine.assets.EraseTexture("doge_gui_window");
        nine_slice.EraseTexture("doge_gui_window");

        engine.assets.EraseTexture("doge_gui_windowex");
        nine_slice.EraseTexture("doge_gui_windowex");
    }
}