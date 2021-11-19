#include <doge/core/Engine.hpp>

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

    lic::Entity& Engine::AddEntity(bool all_scenes) const
    {
        auto& e = lic::AddEntity();
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

    lic::Entity& Engine::AddCamera() const
    {
        auto& e = this->AddEntity();
        e.AddComponent<Camera>();
        return e;
    }
}