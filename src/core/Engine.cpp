#include <doge/core/Engine.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <doge/utils.hpp>
#include <doge/components.hpp>
#include <doge/core/io/Window.hpp>

namespace doge
{
    // IO

    void Engine::CreateWindow()
    {
        window.CreateWindow();

        if (assets.LoadTexture("missing_texture", "missing_texture.png"))
            assets.textures.at("missing_texture").SetRenderOptions(Texture::RenderOptions::Repeated);
    }

    void Engine::CloseWindow()
    {
        assets.Clear();
        window.CloseWindow();
    }

    void Engine::Main()
    {
        scenes.active_scene_id = scenes.current_scene_id;
        auto& active_scene = scenes.scenes.at(scenes.current_scene_id);

        events.on_window_closed.AddListener("Engine_OnWindowClosed", 
        [&]()
        {
            scenes.is_open = false;
            scenes.is_running = false;
        });

        events.on_window_resized.AddListener("Engine_OnWindowResized", 
        [&](const event::Size& size)
        {
            window.settings.size = size.size;
        });

        window.window_io.sf_event.AddListener("events_event_io_OnPollEvent", std::bind(&io::Event::OnPollEvent, events.event_io, std::placeholders::_1));

        scenes.is_running = true;
        if (active_scene.start)
            active_scene.start(*this);
        for (auto [id, extension] : scenes.extensions)
        {
            if (extension.start)
                extension.start(*this);
        }

        DeltaTime acc_fixed_dt = 0;
        DeltaTime dt;
        window.SetFrameRate(window.settings.fps);
        window.window_io.StartDeltaClock();
        while (scenes.active_scene_id == scenes.current_scene_id && scenes.is_running && scenes.is_open)
        {
            dt = window.window_io.GetDeltaTimeRestart();
            acc_fixed_dt += dt;

            window.window_io.PollEvent();
            
            for (; acc_fixed_dt > scenes.fixed_time_step; acc_fixed_dt -= scenes.fixed_time_step)
            {
                if (active_scene.fixed_update)
                    active_scene.fixed_update(*this, scenes.fixed_time_step);
                for (auto [id, extension] : scenes.extensions)
                {
                    if (extension.fixed_update)
                        extension.fixed_update(*this, scenes.fixed_time_step);
                }
            }

            if (active_scene.update)
                active_scene.update(*this, dt);
            for (auto [id, extension] : scenes.extensions)
            {
                if(extension.update)
                    extension.update(*this, dt);
            }

            DestroyEntities();

            window.window_io.Render(*this);
            window.window_io.Display();
        }
        
        for (auto [id, extension] : scenes.extensions)
        {
            if (extension.finish)
                extension.finish(*this);
        }
        if (active_scene.finish)
            active_scene.finish(*this);
        scenes.is_running = false;
        DestroyEntities();

        window.window_io.sf_event.RemoveListener("events_event_io_OnPollEvent");

        events.on_window_closed.RemoveListener("Engine_OnWindowClosed");
    }

    void Engine::StartScene(const std::string& id)
    {
        scenes.current_scene_id = id;
        CreateWindow();

        scenes.is_open = true;
        while (scenes.is_open)
            Main();
    }

    void Engine::StartScene(const std::string& id, const Window::Settings& window_settings)
    {
        window.settings = window_settings;
        StartScene(id);
    }

    void Engine::StopScene()
    {
        window.CloseWindow();
        scenes.is_open = false;
        scenes.is_running = false;
    }

    void Engine::RestartScene(const std::string& new_id)
    {
        scenes.current_scene_id = new_id;
        scenes.is_running = false;
    }

    void Engine::AddScene(const std::string& id, const GameLoopFunctions& glf)
    {
        scenes.scenes.emplace(id, glf);
    }

    // Entities

    void Engine::DestroyEntities()
    {
        while (!to_be_destroyed.empty())
        {
            auto id = to_be_destroyed.front();

            if (!lic::HasEntity(id))
            {
                to_be_destroyed.pop_front();
                continue;
            }

            auto node = GetPCNode(id);
            for (auto& descendent : node->GetDescendents())
            {
                lic::DestroyEntity(descendent->id);
            }
            lic::DestroyEntity(id);
            node->parent->RemoveChild(id);

            to_be_destroyed.pop_front();
        }
    }

    Entity Engine::AddEntity(bool all_scenes)
    {
        auto e = lic::AddEntity();

        if (all_scenes)
            e.AddComponent<SceneInfo>();
        else
            e.AddComponent<SceneInfo>(std::vector<std::string>({ scenes.active_scene_id }));
        
        auto node = PCNode::AddNode(e);

        return Entity(e, node.get());
    }

    Entity Engine::GetEntity(EntityID eid) const
    {
        return Entity(lic::GetEntity(eid), GetPCNode(eid).get());
    }

    bool Engine::HasEntity(EntityID eid) const
    {
        return lic::HasEntity(eid);
    }

    void Engine::DestroyEntity(EntityID eid)
    {
        to_be_destroyed.push_back(eid);
    }

    const std::shared_ptr<PCNode> Engine::GetPCNode(EntityID eid) const
    {
        return PCNode::root.GetDescendent(eid);
    }

    Entity Engine::GetParent(EntityID eid) const
    {
        auto node = GetPCNode(eid);
        if (!node->HasParent())
            throw std::invalid_argument(std::string("Entity ") + std::to_string(eid) + " has no parent.");

        return GetEntity(node->parent->id);
    }

    bool Engine::HasParent(EntityID eid) const
    {
        return GetPCNode(eid)->HasParent();
    }

    bool Engine::IsParent(EntityID eid, EntityID parent) const
    {
        return GetPCNode(eid)->IsParent(parent);
    }
    
    void Engine::SetParent(EntityID eid, EntityID parent)
    {
        GetPCNode(eid)->SetParent(parent);
    }
    
    void Engine::RemoveParent(EntityID eid)
    {
        GetPCNode(eid)->RemoveParent();
    }

    bool Engine::HasChild(EntityID eid, EntityID child) const
    {
        return GetPCNode(eid)->HasChild(child);
    }

    std::vector<Entity> Engine::GetChildren(EntityID eid) const
    {
        auto children_nodes = GetPCNode(eid)->GetChildren();
        std::vector<Entity> children;
        for (auto& child_node : children_nodes)
            children.push_back(GetEntity(child_node->id));
        return children;
    }
}