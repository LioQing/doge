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

        active_scene.start(*this);

        DeltaTime acc_fixed_dt = 0;
        DeltaTime dt;
        sfml_impl.SetFrameRate(fps);
        sfml_impl.StartDeltaClock();
        while (active_scene_id == current_scene_id && is_running)
        {
            dt = sfml_impl.GetDeltaTime();
            acc_fixed_dt += dt;

            sfml_impl.PollEvent([&](const sf::Event& event)
            {
                if (event.type == sf::Event::Closed)
                {
                    is_running = false;
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

            sfml_impl.Render(*this);
            sfml_impl.Display();
        }

        active_scene.finish(*this);
    }

    void Engine::Start(const std::string& id)
    {
        SetCurrentScene(id);

        sfml_impl.CreateWindow(video_settings, title);

        is_running = true;
        while (is_running)
            Main();
    }

    void Engine::Start(const std::string& id, const VideoSettings& video_settings)
    {
        SetVideoSettings(video_settings);
        Start(id);
    }

    void Engine::Stop()
    {
        sfml_impl.CloseWindow();
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

    const Entity Engine::AddEntity(bool all_scenes)
    {
        auto e = Entity(lic::AddEntity());
        if (all_scenes)
            e.AddComponent<SceneInfo>();
        else
            e.AddComponent<SceneInfo>(std::vector<std::string>({ active_scene_id }));
        
        auto node = new PCNode();
        node->parent = &root_parent;
        root_parent.children.emplace(std::make_unique<PCNode>(node));

        return e;
    }

    const Entity Engine::GetEntity(lic::EntityID id) const
    {
        return Entity(lic::GetEntity(id));
    }

    void Engine::DestroyEntity(lic::EntityID eid)
    {
        lic::DestroyEntity(eid);
    }

    void Engine::SetParent(lic::EntityID eid, lic::EntityID parent) 
    {
        auto node = root_parent.GetDescendent(eid);
        auto parent_node = root_parent.GetDescendent(parent);
        if (node->parent)
            node->parent->children.erase(
                std::find_if(node->parent->children.begin(), node->parent->children.end(), 
                [&](const std::unique_ptr<PCNode>& child)
                { return child.get() == node; })
            );
        node->parent = parent_node;
        parent_node->children.emplace(node);
    }

    void Engine::RemoveParent(lic::EntityID eid) 
    {
        if (!HasParent(eid))
            return;

        auto node = root_parent.GetDescendent(eid);
        if (node->parent)
            node->parent->children.erase(
                std::find_if(node->parent->children.begin(), node->parent->children.end(), 
                [&](const std::unique_ptr<PCNode>& child)
                { return child.get() == node; })
            );
        node->parent = &root_parent;
        root_parent.children.emplace(node);
    }

    const Entity Engine::GetParent(lic::EntityID eid)
    {
        auto parent = root_parent.GetDescendent(eid)->parent;
        if (parent)
            return GetEntity(parent->eid);
        throw std::bad_weak_ptr();
    }

    bool Engine::HasParent(lic::EntityID eid)
    {
        auto parent = root_parent.GetDescendent(eid)->parent;
        if (!parent || parent == &root_parent)
            return false;
        return true;
    }

    Entity Engine::EntityContainer::Iterator::operator*() const
    {
        return Entity(lic::GetEntity(VecIterator::operator*()).id);
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::begin() const
    {
        return Iterator(std::vector<lic::EntityID>::cbegin());
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::end() const
    {
        return Iterator(std::vector<lic::EntityID>::cend());
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::cbegin() const
    {
        return Iterator(std::vector<lic::EntityID>::cbegin());
    }

    Engine::EntityContainer::Iterator Engine::EntityContainer::cend() const
    {
        return Iterator(std::vector<lic::EntityID>::cend());
    }
}