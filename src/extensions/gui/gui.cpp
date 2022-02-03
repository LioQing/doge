#include <doge/extensions/gui/GUI.hpp>

#include <doge/core/Engine.hpp>
#include <doge/extensions/nine_slice.hpp>

namespace doge::gui
{
    GUI::GUI(Engine& engine) : engine(engine), nine_slice(engine)
    {
        engine.scenes.extensions.emplace("doge_gui", GameLoopFunctions::Create(*this, &GUI::Start, &GUI::Update, &GUI::FixedUpdate, &GUI::Finish));

        if (engine.scenes.is_running)
        {
            engine.assets.LoadTexture("doge_gui_button", "gui/button.png");
            nine_slice.LoadTexture("doge_gui_button", "gui/button.png", Recti(8, 8, 8, 8));

            engine.assets.LoadTexture("doge_gui_window", "gui/window.png");
            nine_slice.LoadTexture("doge_gui_window", "gui/window.png", Recti(12, 12, 12, 12));
        }
    }

    GUI::~GUI()
    {
        cameras.clear();
        elements.clear();

        engine.scenes.extensions.erase("doge_gui");
    }

    Entity GUI::AddCamera(
        const std::string& id,
        const Rectf& port,
        std::int32_t render_order,
        std::int32_t layer,
        std::size_t layer_width,
        bool destroy_on_finish
    )
    {
        auto [itr, success] = cameras.emplace(id, engine.AddEntity(destroy_on_finish));

        if (!success)
            throw std::invalid_argument("Failed to add camera to gui");

        auto& cam_comp = itr->second.AddComponent(Camera{ .port = port, .render_order = render_order });
        cam_comp.size = GetEngine().window.window_io.GetSize() * port.GetSize();

        GetEngine().events.on_window_resized.AddListener(
            std::string("doge_gui_camera_") + id,
            [this, val_id = id](const event::Size& event)
            { GetCameraComponent(val_id).size = event.size * GetCameraComponent(val_id).port.GetSize(); }
        );

        cam_comp.OnRemoval([&, val_id = id]()
        {
            cameras.erase(val_id);
        });

        std::set<std::int32_t> layers;
        for (std::int32_t i = 0; i < layer_width; ++i)
            layers.emplace(layer + i);

        itr->second.AddComponent<Layer>(layers);

        return itr->second;
    }

    Entity GUI::AddAbsoluteSizeCamera(
        const std::string& id,
        const Rectf& rectangle,
        std::int32_t render_order,
        std::int32_t layer,
        std::size_t layer_width,
        bool destroy_on_finish
    )
    {
        auto [itr, success] = cameras.emplace(id, engine.AddEntity(destroy_on_finish));

        if (!success)
            throw std::invalid_argument("Failed to add camera to gui");

        auto& cam_comp = itr->second.AddComponent(Camera{ .size = rectangle.GetSize(), .render_order = render_order });
        cam_comp.port = Rectf(rectangle.GetPosition() / GetEngine().window.window_io.GetSize(), rectangle.GetSize() / GetEngine().window.window_io.GetSize());

        GetEngine().events.on_window_resized.AddListener(
            std::string("doge_gui_camera_") + id,
            [this, val_id = id](const event::Size& event)
            { GetCameraComponent(val_id).port.SetSize(GetCameraComponent(val_id).size / event.size); }
        );

        cam_comp.OnRemoval([&, val_id = id]()
        {
            cameras.erase(val_id);
        });

        std::set<std::int32_t> layers;
        for (std::int32_t i = 0; i < layer_width; ++i)
            layers.emplace(layer + i);

        itr->second.AddComponent<Layer>(layers);

        return itr->second;
    }

    void GUI::RemoveCamera(const std::string& id)
    {
        GetEngine().events.on_window_resized.RemoveListener(std::string("doge_gui_camera_") + id);
        GetEngine().DestroyEntity(GetCameraEntity(id));
    }

    doge::Component<Camera>& GUI::GetCameraComponent(const std::string& id) const
    {
        return cameras.at(id).GetComponent<Camera>();
    }

    Entity GUI::GetCameraEntity(const std::string& id) const
    {
        return cameras.at(id);
    }

    std::int32_t GUI::GetCameraLayer(const std::string& id) const
    {
        return GetCameraEntity(id).GetComponent<Layer>().layers.begin().operator*();
    }

    const std::set<std::int32_t>& GUI::GetCameraLayers(const std::string& id) const
    {
        return GetCameraEntity(id).GetComponent<Layer>().layers;
    }

    std::int32_t GUI::GetCameraRenderOrder(const std::string& id) const
    {
        return GetCameraComponent(id).render_order;
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
        for (auto& [id, element] : elements)
        {
            if (element.GetComponent<Component>().element->GetCameraID() == camera_id)
                engine.DestroyEntity(element);
        }
    }

    Element& GUI::GetElement(const std::string& id) const
    {
        return *GetElementComponent(id).element;
    }

    doge::Component<Component>& GUI::GetElementComponent(const std::string& id) const
    {
        return elements.at(id).GetComponent<Component>();
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
            auto& ptr = element.GetComponent<Component>().element;
            Vec2f cursor_pos = GetEngine().window.MapPixelToCoords(GetEngine().window.window_io.GetMousePosition(), ptr->GetCameraComponent());
            Recti cam_rect = Recti(
                ptr->GetCameraComponent().port.GetPosition() * GetEngine().window.window_io.GetSize(),
                ptr->GetCameraComponent().port.GetSize() * GetEngine().window.window_io.GetSize()
            );

            if (
                math::TestPoint(GetEngine().window.window_io.GetMousePosition(), cam_rect) &&
                math::TestPoint(cursor_pos, ptr->GetRectangle()) &&
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
    }

    void GUI::Update(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<Component>().element->Update(dt);
        }
    }

    void GUI::FixedUpdate(Engine& engine, DeltaTime dt)
    {
        for (auto& [id, element] : elements)
        {
            element.GetComponent<Component>().element->FixedUpdate(dt);
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
    }
}