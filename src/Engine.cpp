#include <doge/core/Engine.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <doge/utils.hpp>
#include <doge/components.hpp>

namespace doge
{
    // IO

    void Engine::CreateWindow()
    {
        io_bus.CreateWindow(window_settings);

        assets.LoadTexture("missing_texture", "missing_texture.png");
        assets.textures.at("missing_texture").SetRenderOptions(Texture::RenderOptions::Repeated);
    }

    void Engine::CloseWindow()
    {
        assets.Clear();
        io_bus.CloseWindow();
    }

    void Engine::SetFrameRate(uint32_t fps)
    {
        window_settings.fps = fps;
        io_bus.SetFrameRate(fps);
    }

    // Game Loop

    void Engine::Main()
    {
        active_scene_id = current_scene_id;
        auto active_scene = scenes.at(current_scene_id);

        is_running = true;
        if (active_scene.start)
            active_scene.start(*this);
        for (auto [id, extension] : extensions)
        {
            if (extension.start)
                extension.start(*this);
        }

        DeltaTime acc_fixed_dt = 0;
        DeltaTime dt;
        io_bus.SetFrameRate(window_settings.fps);
        io_bus.StartDeltaClock();
        while (active_scene_id == current_scene_id && is_running && is_open)
        {
            dt = io_bus.GetDeltaTime();
            acc_fixed_dt += dt;

            io_bus.PollEvent([&](const sf::Event& event)
            {
                if (event.type == sf::Event::Closed)
                {
                    is_running = false;
                    is_open = false;
                }
                else if (event.type == sf::Event::Resized)
                {
                    window_settings.resolution = Vec2u(event.size.width, event.size.height);
                }
            });
            
            for (; acc_fixed_dt > fixed_time_step; acc_fixed_dt -= fixed_time_step)
            {
                if (active_scene.fixed_update)
                    active_scene.fixed_update(*this, fixed_time_step);
                for (auto [id, extension] : extensions)
                {
                    if (extension.fixed_update)
                        extension.fixed_update(*this, fixed_time_step);
                }
            }

            if (active_scene.update)
                active_scene.update(*this, dt);
            for (auto [id, extension] : extensions)
            {
                if(extension.update)
                    extension.update(*this, dt);
            }

            DestroyEntities();

            io_bus.Render(*this);
            io_bus.Display();
        }

        
        for (auto [id, extension] : extensions)
        {
            if (extension.finish)
                extension.finish(*this);
        }
        if (active_scene.finish)
            active_scene.finish(*this);
        is_running = false;
        DestroyEntities();
    }

    void Engine::StartScene(const std::string& id)
    {
        SetCurrentScene(id);
        CreateWindow();

        is_open = true;
        while (is_open)
            Main();
    }

    void Engine::StartScene(const std::string& id, const WindowSettings& window_settings)
    {
        this->window_settings = window_settings;
        StartScene(id);
    }

    void Engine::StopScene()
    {
        io_bus.CloseWindow();
        is_open = false;
        is_running = false;
    }

    void Engine::RestartScene()
    {
        is_running = false;
    }

    void Engine::AddScene(const std::string& id, const GameLoopFunctions& glf)
    {
        scenes.emplace(id, glf);
    }

    void Engine::SetCurrentScene(const std::string& id)
    {
        if (scenes.find(id) == scenes.end())
        {
            throw std::out_of_range("Scene ID \"" + id + "\" not found");
        }

        current_scene_id = id;
    }

    bool Engine::HasScene(const std::string& id) const
    {
        return scenes.find(id) != scenes.end();
    }

    const std::string& Engine::GetCurrentScene() const 
    {
        return current_scene_id;
    }

    const std::string& Engine::GetActiveScene() const 
    {
        return active_scene_id;
    }

    void Engine::SetFixedTimeStep(float millisec)
    {
        fixed_time_step = millisec;
    }

    void Engine::AddExtension(const std::string& id, const GameLoopFunctions& glf)
    {
        extensions.emplace(id, glf);
    }

    void Engine::EraseExtension(const std::string& id)
    {
        extensions.erase(id);
    }

    bool Engine::HasExtension(const std::string& id) const
    {
        return extensions.find(id) != extensions.end();
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
            e.AddComponent<SceneInfo>(std::vector<std::string>({ active_scene_id }));
        
        auto node = PCNode::AddNode(e);

        return Entity(e, node.get());
    }

    Entity Engine::GetEntity(EntityID eid) const
    {
        return Entity(lic::GetEntity(eid), GetPCNode(eid).get());
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