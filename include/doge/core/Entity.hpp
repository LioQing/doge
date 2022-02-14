#pragma once

#include <vector>
#include <doge/utils/lic.hpp>
#include <doge/utils/aliases.hpp>

namespace doge
{
    struct Engine;
    struct PCNode;
    template <typename TComp>
    struct Component;

    struct Entity : public lic::Entity
    {
        Entity() = default;
        Entity(EntityID EntityID, PCNode* pcnode);

        template <typename TComp, typename... TArgs>
        Component<TComp>& AddComponent(TArgs&&... args) const
        {
            return static_cast<Component<TComp>&>(lic::Entity::AddComponent<TComp>(args...));
        }

        template <typename TComp>
        Component<TComp>& AddComponent(const TComp& c) const
        {
            return static_cast<Component<TComp>&>(lic::Entity::AddComponent<TComp>(c));
        }
        
        template <typename TComp>
        Component<TComp>& GetComponent() const
        {
            return static_cast<Component<TComp>&>(lic::Entity::GetComponent<TComp>());
        }

        Entity GetParent() const;
        bool HasParent() const;
        bool IsParent(EntityID parent) const;
        void SetParent(EntityID parent) const;
        void RemoveParent() const;

        bool HasChild(EntityID child) const;
        std::vector<Entity> GetChildren() const;

        operator EntityID() const;
    
    private:

        PCNode* pcnode = nullptr;
    };
}