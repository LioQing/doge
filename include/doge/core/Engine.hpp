#pragma once

#include <string>
#include <vector>
#include <concepts>
#include <functional>
#include <iostream>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <doge/core/Window.hpp>
#include <doge/core/Entity.hpp>
#include <doge/core/PCNode.hpp>
#include <doge/core/Range.hpp>
#include <doge/core/Assets.hpp>
#include <doge/core/Scenes.hpp>
#include <doge/core/Events.hpp>

namespace doge
{
    struct GameLoopFunctions;

    struct Engine
    {
        // IO

        Window window;

        void CreateWindow();
        void CloseWindow();

        // Game loop

        Scenes scenes;

        void StartScene(const std::string& id);
        void StartScene(const std::string& id, const Window::Settings& window_settings);

        void StopScene();

        void RestartScene(const std::string& new_id);

        void AddScene(const std::string& id, const GameLoopFunctions& glf);

        // Entities

        std::deque<EntityID> to_be_destroyed;

        Entity AddEntity(bool destroy_on_finish = true);

        Entity GetEntity(EntityID eid) const;

        bool HasEntity(EntityID eid) const;

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
        Range<TComps...> Select(bool include_disabled = false) const
        {
            if (include_disabled)
                return Range<TComps...>(lic::Select<TComps...>());
            
            return Range<TComps...>(
                lic::Select<TComps...>()
                .Where(
                    [&](lic::Entity entity, TComps... c)
                    { return global::IsEnabled(GetEntity(entity.id)); }
                )
            );
        }

        // assets
        Assets assets;

        // events
        Events events;

    private:

        // Helper functions
        void Main();
        void DestroyEntities();
        const std::shared_ptr<PCNode> GetPCNode(EntityID eid) const;
    };
}