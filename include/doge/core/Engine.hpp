#pragma once

#include <string>
#include <concepts>
#include <functional>
#include <iostream>
#include <unordered_map>

#include "../utils.hpp"
#include "../components/SceneInfo.hpp"
#include "SFMLImpl.hpp"
#include "Scene.hpp"
#include "VideoSettings.hpp"
#include "Entity.hpp"

namespace doge
{
    struct Engine
    {
    private:
    
        SFMLImpl sfml_impl;
        VideoSettings video_settings;
        std::string title = "";

        std::unordered_map<std::string, Scene> scenes;
        std::unordered_multimap<lic::EntityID, lic::EntityID> parental_tree;
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

        const Entity AddEntity(bool all_scenes = false) const;

        template <std::convertible_to<std::string>... T>
        const Entity AddEntity(T... scene_ids) const
        {
            auto e = Entity(lic::AddEntity());
            e.AddComponent<SceneInfo>(std::vector<std::string>({ scene_ids... }));
            return e;
        }

        void DestroyEntity(lic::EntityID eid) const;

        template <typename... TArgs>
        const Entity AddCamera(TArgs&&... args) const
        {
            auto e = this->AddEntity();
            e.AddComponent<Camera>(std::forward<TArgs>(args)...);
            return e;
        }

        void SetParent(lic::EntityID eid, lic::EntityID parent);
        void RemoveParent(lic::EntityID eid);
        Entity GetParent(lic::EntityID eid);
        bool HasParent(lic::EntityID eid);

        struct EntityContainer : public std::vector<lic::EntityID>
        {
        private:

            using VecIterator = std::vector<lic::EntityID>::const_iterator;

        public:

            struct Iterator : public VecIterator
            {
                Iterator(const VecIterator& iter) : VecIterator(iter) {}

                Entity operator*() const;
            };

            Iterator begin() const;
            Iterator end() const;
            Iterator cbegin() const;
            Iterator cend() const;
        };

        template <bool IncludeEntities, typename... TComps>
        struct ComponentContainer : public std::vector<lic::EntityID>
        {
            template <std::contiguous_iterator TBackingIter>
            struct BaseIterator : public TBackingIter
            {
                BaseIterator(const TBackingIter& iter) : TBackingIter(iter) {}

                auto operator*()
                {
                    if constexpr (IncludeEntities == true)
                    {
                        if constexpr (std::is_same<TBackingIter, std::vector<lic::EntityID>::const_iterator>::value)
                            return std::tuple<Entity, const lic::Component<TComps>&...>(lic::GetEntity(TBackingIter::operator*()), lic::GetComponent<TComps>(TBackingIter::operator*())...);
                        else
                            return std::tuple<Entity, lic::Component<TComps>&...>(lic::GetEntity(TBackingIter::operator*()), lic::GetComponent<TComps>(TBackingIter::operator*())...);
                    }
                    else
                    {
                        if constexpr (std::is_same<TBackingIter, std::vector<lic::EntityID>::const_iterator>::value)
                            return std::tie<const lic::Component<TComps>...>(lic::GetComponent<TComps>(TBackingIter::operator*())...);
                        else
                            return std::tie<lic::Component<TComps>...>(lic::GetComponent<TComps>(TBackingIter::operator*())...);
                    }
                }
            };

            using Iterator = BaseIterator<std::vector<lic::EntityID>::iterator>;
            using ConstIterator = BaseIterator<std::vector<lic::EntityID>::const_iterator>;

            Iterator begin() { return Iterator(std::vector<lic::EntityID>::begin()); }
            Iterator end() { return Iterator(std::vector<lic::EntityID>::end()); }

            ConstIterator begin() const { return ConstIterator(std::vector<lic::EntityID>::cbegin()); }
            ConstIterator end() const { return ConstIterator(std::vector<lic::EntityID>::cend()); }
        };

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

            EntityContainer Entities() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).Entities();
                return EntityContainer(lic::Range<SceneInfo, TComps...>::entities);
            }

            ComponentContainer<false, TComps...> Components()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).Components();
                return ComponentContainer<false, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            const ComponentContainer<false, TComps...> Components() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).Components();
                return ComponentContainer<false, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            template <typename... TOnlyComps>
            ComponentContainer<false, TOnlyComps...> OnlyComponents()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).OnlyComponents<TOnlyComps...>();
                return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            template <typename... TOnlyComps>
            const ComponentContainer<false, TOnlyComps...> OnlyComponents() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).OnlyComponents<TOnlyComps...>();
                return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            ComponentContainer<true, TComps...> EntitiesAndComponents()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndComponents();
                return ComponentContainer<true, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            const ComponentContainer<true, TComps...> EntitiesAndComponents() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndComponents();
                return ComponentContainer<true, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            template <typename... TOnlyComps>
            ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents()
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndOnlyComponents<TOnlyComps...>();
                return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }

            template <typename... TOnlyComps>
            const ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents() const
            {
                if (!current_scene.empty())
                    return InAnyOf(current_scene).EntitiesAndOnlyComponents<TOnlyComps...>();
                return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
            }
        };

        template <typename... TComps>
        Range<TComps...> Select() const
        {
            return Range<TComps...>(lic::Select<SceneInfo, TComps...>(), current_scene);
        }
    };
}