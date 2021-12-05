#include <doge/core/Engine.hpp>

#include <algorithm>
#include <stdexcept>
#include <vector>
#include <doge/utils.hpp>
#include <doge/components.hpp>

namespace doge
{
    void Engine::Main()
    {
        active_scene_id = current_scene_id;
        auto active_scene = scenes.at(current_scene_id);

        is_running = true;
        active_scene.start(*this);

        DeltaTime acc_fixed_dt = 0;
        DeltaTime dt;
        sfml_impl.SetFrameRate(fps);
        sfml_impl.StartDeltaClock();
        while (active_scene_id == current_scene_id && is_running && is_open)
        {
            dt = sfml_impl.GetDeltaTime();
            acc_fixed_dt += dt;

            sfml_impl.PollEvent([&](const sf::Event& event)
            {
                if (event.type == sf::Event::Closed)
                {
                    is_running = false;
                    is_open = false;
                }
                else if (event.type == sf::Event::Resized)
                {
                    video_settings.resolution = Vec2u(event.size.width, event.size.height);
                }
            });
            
            for (; acc_fixed_dt > fixed_time_step; acc_fixed_dt -= fixed_time_step)
            {
                active_scene.fixed_update(*this, fixed_time_step);
            }

            active_scene.update(*this, dt);

            DestroyEntities();

            sfml_impl.Render(*this);
            sfml_impl.Display();
        }

        active_scene.finish(*this);
        is_running = false;
    }

    void Engine::DestroyEntities()
    {
        for (auto id : to_be_destroyed)
        {
            if (!lic::HasEntity(id))
                continue;

            auto node = GetPCNode(id);
            for (auto& descendents : node->GetDescendents())
                lic::DestroyEntity(descendents->id);
            lic::DestroyEntity(id);
            node->parent->RemoveChild(id);
        }

        to_be_destroyed.clear();
    }

    void Engine::StartScene(const std::string& id)
    {
        SetCurrentScene(id);

        sfml_impl.CreateWindow(video_settings, title);

        is_open = true;
        while (is_open)
            Main();
    }

    void Engine::StartScene(const std::string& id, const VideoSettings& video_settings)
    {
        SetVideoSettings(video_settings);
        StartScene(id);
    }

    void Engine::StopScene()
    {
        sfml_impl.CloseWindow();
        is_open = false;
        is_running = false;
    }

    void Engine::RestartScene()
    {
        is_running = false;
    }

    void Engine::SetFrameRate(uint32_t frame_rate)
    {
        fps = frame_rate;
    }

    void Engine::SetFixedTimeStep(float millisec)
    {
        fixed_time_step = millisec;
    }

    void Engine::SetCurrentScene(const std::string& id)
    {
        if (scenes.find(id) == scenes.end())
        {
            throw std::out_of_range("Scene ID \"" + id + "\" not found");
        }

        current_scene_id = id;
    }

    const std::string& Engine::GetCurrentScene() const 
    {
        return current_scene_id;
    }

    const std::string& Engine::GetActiveScene() const 
    {
        return active_scene_id;
    }

    void Engine::SetVideoSettings(const VideoSettings& video_settings)
    {
        this->video_settings = video_settings;
        sfml_impl.CreateWindow(video_settings, title);
    }

    const VideoSettings& Engine::GetVideoSettings() const
    {
        return this->video_settings;
    }

    void Engine::SetTitle(const std::string& title)
    {
        this->title = title;
    }

    Entity Engine::AddEntity(bool all_scenes)
    {
        auto e = Entity(lic::AddEntity());

        if (all_scenes)
            e.AddComponent<SceneInfo>();
        else
            e.AddComponent<SceneInfo>(std::vector<std::string>({ active_scene_id }));
        
        PCNode::AddNode(e);

        return e;
    }

    Entity Engine::GetEntity(EntityID eid) const
    {
        return Entity(lic::GetEntity(eid));
    }

    void Engine::DestroyEntity(EntityID eid)
    {
        to_be_destroyed.push_back(eid);

        if (!is_running)
            DestroyEntities();
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

    const std::vector<Entity> Engine::GetChildren(EntityID eid) const
    {
        auto children_nodes = GetPCNode(eid)->GetChildren();
        std::vector<Entity> children;
        for (auto& child_node : children_nodes)
            children.push_back(GetEntity(child_node->id));
        return children;
    }

    void Engine::RemoveChild(EntityID eid, EntityID child)
    {
        return GetPCNode(eid)->RemoveChild(child);
    }

    Entity Engine::EntityContainer::Iterator::operator*() const
    {
        return Entity(lic::GetEntity(VecIterator::operator*()).id);
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::begin() const
    {
        return Iterator(std::vector<EntityID>::cbegin());
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::end() const
    {
        return Iterator(std::vector<EntityID>::cend());
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::cbegin() const
    {
        return Iterator(std::vector<EntityID>::cbegin());
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::cend() const
    {
        return Iterator(std::vector<EntityID>::cend());
    }
}