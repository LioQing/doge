#include <doge/core/Engine.hpp>

#include <vector>
#include <doge/utils.hpp>
#include <doge/components.hpp>

namespace doge
{
    void Engine::Main()
    {
        auto running_scene = current_scene;
        auto active_scene = scenes.at(current_scene);

        active_scene.start(*this);

        DeltaTime acc_fixed_dt = 0;
        DeltaTime dt;
        sfml_impl.SetFrameRate(fps);
        sfml_impl.StartDeltaClock();
        while (running_scene == current_scene && is_running)
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
    }

    void Engine::Start(const std::string& id)
    {
        SetScene(id);

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

    void Engine::SetScene(const std::string& id)
    {
        if (scenes.find(id) == scenes.end())
        {
            throw std::out_of_range("Scene ID \"" + id + "\" not found");
        }

        current_scene = id;
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

    const Entity Engine::AddEntity(bool all_scenes) const
    {
        auto e = Entity(lic::AddEntity());
        if (all_scenes)
            e.AddComponent<SceneInfo>();
        else
            e.AddComponent<SceneInfo>(std::vector<std::string>({ current_scene }));
        return e;
    }

    void Engine::DestroyEntity(lic::EntityID eid) const
    {
        lic::DestroyEntity(eid);
    }

    void Engine::SetParent(lic::EntityID eid, lic::EntityID parent) 
    {
        parental_tree.emplace(parent, eid);
    }

    void Engine::RemoveParent(lic::EntityID eid) 
    {
        auto itr = parental_tree.find(GetParent(eid));
        
    }

    Entity Engine::GetParent(lic::EntityID eid) 
    {
        
    }

    bool Engine::HasParent(lic::EntityID eid) 
    {
        
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