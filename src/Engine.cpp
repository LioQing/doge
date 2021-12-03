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

            sfml_impl.Render(*this);
            sfml_impl.Display();
        }

        active_scene.finish(*this);
        is_running = false;
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

    const Entity Engine::AddEntity(bool all_scenes)
    {
        auto e = Entity(lic::AddEntity());

        if (all_scenes)
            e.AddComponent<SceneInfo>();
        else
            e.AddComponent<SceneInfo>(std::vector<std::string>({ active_scene_id }));
        
        PCNode::AddNode(e);

        return e;
    }

    const Entity Engine::GetEntity(lic::EntityID id) const
    {
        return Entity(lic::GetEntity(id));
    }

    void Engine::DestroyEntity(lic::EntityID id)
    {
        lic::DestroyEntity(id);
        PCNode::root.GetDescendent(id)->parent->RemoveChild(id);
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