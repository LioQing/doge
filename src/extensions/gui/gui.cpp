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

    std::shared_ptr<Element> GUI::GetElementBelowCursor() const
    {
        std::shared_ptr<Element> ret_ptr = nullptr;

        for (auto& [id, element] : elements)
        {
            auto& ptr = element.GetComponent<ElementComponent>().element;

            if (!ptr->IsCursorDetectable())
                continue;

            Vec2f cursor_pos = GetEngine().window.MapPixelToCoords(GetEngine().window.window_io.GetMousePosition(), ptr->GetCameraComponent());
            Recti cam_rect = Recti(
                ptr->GetCameraComponent().port.GetPosition() * GetEngine().window.window_io.GetSize(),
                ptr->GetCameraComponent().port.GetSize() * GetEngine().window.window_io.GetSize()
            );

            if (
                math::TestPoint(GetEngine().window.window_io.GetMousePosition(), cam_rect) &&
                ptr->TestPoint(cursor_pos) &&
                (!ret_ptr || ptr->GetLayer() > ret_ptr->GetLayer())
            )
            {
                ret_ptr = ptr;
            }
        }

        return ret_ptr;
    }

    void GUI::Start(Engine& engine)
    {
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
    }

    void GUI::Update(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<ElementComponent>().element->Update(dt);
        }
    }

    void GUI::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<ElementComponent>().element->FixedUpdate(dt);
        }
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