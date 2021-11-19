#pragma once

#include <string>
#include <concepts>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "../utils/lic.hpp"
#include "../components/SceneInfo.hpp"
#include "SFMLImpl.hpp"
#include "Scene.hpp"
#include "VideoSettings.hpp"

namespace doge
{
    struct Engine
    {
    private:
    
        SFMLImpl sfml_impl;
        VideoSettings video_settings;
        std::string title = "";

        std::unordered_map<std::string, Scene> scenes;
        std::string current_scene;
        DeltaTime fixed_time_step = 10.f;
        uint32_t fps = 60;
        bool is_running = false;

        void Main();

    public:

        void Start(const std::string& id);
        void Start(const std::string& id, const VideoSettings& video_settings);

        void Stop();

        void SetFrameRate(uint32_t frame_rate);

        void SetFixedTimeStep(float millisec);

        template <typename TStartFunctor, typename TUpdateFunctor>
        requires std::invocable<TStartFunctor, Engine&> && std::invocable<TUpdateFunctor, Engine&, DeltaTime>
        void AddScene(const std::string& id, TStartFunctor start, TUpdateFunctor update, TUpdateFunctor fixed_update = [](Engine&, DeltaTime){})
        {
            scenes.emplace(id, Scene(start, update, fixed_update));
        }

        void SetScene(const std::string& id);

        const std::string& GetCurrentScene() const;

        void SetVideoSettings(const VideoSettings& video_settings);

        const VideoSettings& GetVideoSettings() const;

        void SetTitle(const std::string& title);

        lic::Entity& AddEntity(bool all_scenes = false) const;

        template <std::convertible_to<std::string>... T>
        lic::Entity& AddEntity(T... scene_ids) const
        {
            auto& e = lic::AddEntity();
            e.AddComponent<SceneInfo>(std::vector<std::string>({ scene_ids... }));
            return e;
        }

        void DestroyEntity(lic::EntityID eid) const;

        lic::Entity& AddCamera() const;

        template <typename... TComps>
        struct Range : public lic::Range<SceneInfo, TComps...>
        {
            std::string current_scene = "";

            template <std::convertible_to<std::string>... TSceneID>
            Range<TComps...> InAnyOf(TSceneID&&... scene_ids) const
            {
                return Range(lic::Range<SceneInfo, TComps...>::Where(
                    [&](SceneInfo scene_info, TComps... _)
                    { 
                        for (auto& scene_id : { scene_ids... })
                            if (std::find(scene_info.scene_ids.begin(), scene_info.scene_ids.end(), scene_id) != scene_info.scene_ids.end())
                                return true;
                        return false;
                    }));
            }

            template <std::convertible_to<std::string>... TSceneID>
            Range<TComps...> InAllOf(TSceneID&&... scene_ids) const
            {
                return Range(lic::Range<SceneInfo, TComps...>::Where(
                    [&](SceneInfo scene_info, TComps... _)
                    { 
                        for (auto& scene_id : { scene_ids... })
                            if (std::find(scene_info.scene_ids.begin(), scene_info.scene_ids.end(), scene_id) == scene_info.scene_ids.end())
                                return false;
                        return true;
                    }));
            }

            template <std::convertible_to<std::string>... TSceneID>
            Range<TComps...> InNoneOf(TSceneID&&... scene_ids) const
            {
                return Range(lic::Range<SceneInfo, TComps...>::Where(
                    [&](SceneInfo scene_info, TComps... _)
                    {
                        for (auto& scene_id : { scene_ids... })
                            if (std::find(scene_info.scene_ids.begin(), scene_info.scene_ids.end(), scene_id) != scene_info.scene_ids.end())
                                return false;
                        return true;
                    }));
            }

            template <typename... TSelectComps>
            Range<TComps..., TSelectComps...> Select() const
            {
                return Range(lic::Range<SceneInfo, TComps...>::Select<TSelectComps...>());
            }

            template <std::predicate<TComps...> TPred>
            Range<TComps...> Where(TPred predicate) const
            {
                return Range(lic::Range<SceneInfo, TComps...>::Where([&](SceneInfo _, TComps... c){ return predicate(c...); }));
            }

            lic::EntityContainer Entities() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).Entities();
                return lic::Range<SceneInfo, TComps...>::Entities();
            }

            lic::ComponentContainer<false, TComps...> Components()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).Components();
                return lic::Range<SceneInfo, TComps...>::template OnlyComponents<TComps...>();
            }

            const lic::ComponentContainer<false, TComps...> Components() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).Components();
                return lic::Range<SceneInfo, TComps...>::template OnlyComponents<TComps...>();
            }

            template <typename... TOnlyComps>
            lic::ComponentContainer<false, TOnlyComps...> OnlyComponents()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).OnlyComponents<TOnlyComps...>();
                return lic::Range<SceneInfo, TComps...>::template OnlyComponents<TOnlyComps...>();
            }

            template <typename... TOnlyComps>
            const lic::ComponentContainer<false, TOnlyComps...> OnlyComponents() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).OnlyComponents<TOnlyComps...>();
                return lic::Range<SceneInfo, TComps...>::template OnlyComponents<TOnlyComps...>();
            }

            lic::ComponentContainer<true, TComps...> EntitiesAndComponents()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndComponents();
                return lic::Range<SceneInfo, TComps...>::template EntitiesAndOnlyComponents<TComps...>();
            }

            const lic::ComponentContainer<true, TComps...> EntitiesAndComponents() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndComponents();
                return lic::Range<SceneInfo, TComps...>::template EntitiesAndOnlyComponents<TComps...>();
            }

            template <typename... TOnlyComps>
            lic::ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndOnlyComponents<TOnlyComps...>();
                return lic::Range<SceneInfo, TComps...>::template EntitiesAndOnlyComponents<TOnlyComps...>();
            }

            template <typename... TOnlyComps>
            const lic::ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndOnlyComponents<TOnlyComps...>();
                return lic::Range<SceneInfo, TComps...>::template EntitiesAndOnlyComponents<TOnlyComps...>();
            }
        };

        template <typename... TComps>
        Range<TComps...> Select() const
        {
            return Range<TComps...>(lic::Select<SceneInfo, TComps...>(), current_scene);
        }
    };
}