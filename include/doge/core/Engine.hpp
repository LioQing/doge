#pragma once

#include <string>
#include <vector>
#include <concepts>
#include <functional>
#include <iostream>
#include <unordered_map>
#include <unordered_set>

#include "../utils.hpp"
#include "../components/SceneInfo.hpp"
#include "IOBus.hpp"
#include "Scene.hpp"
#include "VideoSettings.hpp"
#include "Entity.hpp"
#include "PCNode.hpp"
#include "Range.hpp"

namespace doge
{
    struct Engine
    {
    private:
    
        // IO
        IOBus io_bus;
        VideoSettings video_settings;
        std::string title = "";

        // Game Loop
        DeltaTime fixed_time_step = 10.f;
        std::unordered_map<std::string, Scene> scenes;
        bool is_open = false;
        bool is_running = false;

        // Entities
        std::vector<EntityID> to_be_destroyed;
        std::string current_scene_id;
        std::string active_scene_id;

        // Helper functions
        void Main();
        void DestroyEntities();
        const std::shared_ptr<PCNode> GetPCNode(lic::EntityID eid) const;

    public:

        // IO

        void SetVideoSettings(const VideoSettings& video_settings);

        const VideoSettings& GetVideoSettings() const;

        void SetFrameRate(uint32_t frame_rate);

        void SetFixedTimeStep(float millisec);

        void SetTitle(const std::string& title);

        // Game loop

        void StartScene(const std::string& id);
        void StartScene(const std::string& id, const VideoSettings& video_settings);

        void StopScene();

        void RestartScene();

        template <typename TCallFunctor, typename TUpdateFunctor>
        requires std::invocable<TCallFunctor, Engine&> && std::invocable<TUpdateFunctor, Engine&, DeltaTime>
        void AddScene(const std::string& id, 
            TCallFunctor start, 
            TUpdateFunctor update, 
            TUpdateFunctor fixed_update = [](Engine&, DeltaTime){}, 
            TCallFunctor finish = default_functions::Finish)
        {
            scenes.emplace(id, Scene(start, update, fixed_update, finish));
        }

        void SetCurrentScene(const std::string& id);

        const std::string& GetCurrentScene() const;
        const std::string& GetActiveScene() const;

        // Entities

        Entity AddEntity(bool all_scenes = false);

        template <std::convertible_to<std::string>... T>
        Entity AddEntity(T... scene_ids)
        {
            auto e = lic::AddEntity();
            e.AddComponent<SceneInfo>(std::vector<std::string>({ scene_ids... }));
        
            auto node = PCNode::AddNode(e);

            return Entity(e, node.get());
        }

        Entity GetEntity(EntityID eid) const;

        void DestroyEntity(EntityID eid);

        template <typename... TArgs>
        Entity AddCamera(TArgs&&... args)
        {
            auto e = this->AddEntity();
            e.AddComponent<Camera>(std::forward<TArgs>(args)...);
            return e;
        }

        Entity GetParent(EntityID eid) const;
        bool HasParent(EntityID eid) const;
        bool IsParent(EntityID eid, EntityID parent) const;
        void SetParent(EntityID eid, EntityID parent);
        void RemoveParent(EntityID eid);

        bool HasChild(EntityID eid, EntityID child) const;
        std::vector<Entity> GetChildren(EntityID eid) const;

        template <typename... TComps>
        Range<TComps...> Select() const
        {
            return Range<TComps...>(lic::Select<SceneInfo, TComps...>(), active_scene_id);
        }
    };
}