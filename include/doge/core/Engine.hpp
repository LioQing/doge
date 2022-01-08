#pragma once

#include <string>
#include <vector>
#include <concepts>
#include <functional>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <unordered_set>

#include <doge/utils.hpp>
#include <doge/components/SceneInfo.hpp>
#include <doge/core/IOBus.hpp>
#include <doge/core/GameLoopFunctions.hpp>
#include <doge/core/WindowSettings.hpp>
#include <doge/core/Entity.hpp>
#include <doge/core/PCNode.hpp>
#include <doge/core/Range.hpp>
#include <doge/core/Assets.hpp>

namespace doge
{
    struct Engine
    {
    private:
    
        // IO
        IOBus io_bus;

        // Game Loop
        DeltaTime fixed_time_step = 10.f;
        std::unordered_map<std::string, GameLoopFunctions> extensions;
        std::unordered_map<std::string, GameLoopFunctions> scenes;
        std::string current_scene_id;
        std::string active_scene_id;
        bool is_open = false;
        bool is_running = false;

        // Entities
        std::deque<EntityID> to_be_destroyed;

        // Helper functions
        void Main();
        void DestroyEntities();
        const std::shared_ptr<PCNode> GetPCNode(lic::EntityID eid) const;

    public:

        // IO

        WindowSettings window_settings;

        void CreateWindow();
        void CloseWindow();

        void SetFrameRate(uint32_t fps);

        // Game loop

        void StartScene(const std::string& id);
        void StartScene(const std::string& id, const WindowSettings& window_settings);

        void StopScene();

        void RestartScene();

        void AddScene(const std::string& id, const GameLoopFunctions& glf);

        void SetCurrentScene(const std::string& id);

        bool HasScene(const std::string& id) const;

        const std::string& GetCurrentScene() const;
        const std::string& GetActiveScene() const;

        void SetFixedTimeStep(float millisec);

        void AddExtension(const std::string& id, const GameLoopFunctions& glf);

        void EraseExtension(const std::string& id);

        bool HasExtension(const std::string& id) const;

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

        // assets

        Assets assets;
    };
}