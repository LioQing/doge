#pragma once

#include <vector>

#include "Entity.hpp"
#include "Component.hpp"
#include "../utils.hpp"

namespace doge
{
    struct EntityContainer : public std::vector<EntityID>
    {
    private:

        using VecIterator = std::vector<EntityID>::const_iterator;

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
    struct ComponentContainer : public std::vector<EntityID>
    {
        template <std::contiguous_iterator TBackingIter>
        struct BaseIterator : public TBackingIter
        {
            BaseIterator(const TBackingIter& iter) : TBackingIter(iter) {}

            auto operator*()
            {
                EntityID eid = lic::GetEntity(TBackingIter::operator*()).id;

                if constexpr (IncludeEntities == true)
                {
                    if constexpr (std::is_same<TBackingIter, std::vector<EntityID>::const_iterator>::value)
                        return std::tuple<Entity, const Component<TComps>&...>
                        (Entity(eid, PCNode::root.GetDescendent(eid).get()), static_cast<const Component<TComps>&>(lic::GetComponent<TComps>(eid))...);
                    else
                        return std::tuple<Entity, Component<TComps>&...>
                        (Entity(eid, PCNode::root.GetDescendent(eid).get()), static_cast<Component<TComps>&>(lic::GetComponent<TComps>(eid))...);
                }
                else
                {
                    if constexpr (std::is_same<TBackingIter, std::vector<EntityID>::const_iterator>::value)
                        return std::tie<const Component<TComps>...>(static_cast<Component<TComps>&>(lic::GetComponent<TComps>(eid))...);
                    else
                        return std::tie<Component<TComps>...>(static_cast<Component<TComps>&>(lic::GetComponent<TComps>(eid))...);
                }
            }
        };

        using Iterator = BaseIterator<std::vector<EntityID>::iterator>;
        using ConstIterator = BaseIterator<std::vector<EntityID>::const_iterator>;

        Iterator begin() { return Iterator(std::vector<EntityID>::begin()); }
        Iterator end() { return Iterator(std::vector<EntityID>::end()); }

        ConstIterator begin() const { return ConstIterator(std::vector<EntityID>::cbegin()); }
        ConstIterator end() const { return ConstIterator(std::vector<EntityID>::cend()); }
    };

    template <typename... TComps>
    struct Range : public lic::Range<SceneInfo, TComps...>
    {
        std::string active_scene_id = "";

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
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).Entities();
            return EntityContainer(lic::Range<SceneInfo, TComps...>::entities);
        }

        ComponentContainer<false, TComps...> Components()
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).Components();
            return ComponentContainer<false, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        const ComponentContainer<false, TComps...> Components() const
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).Components();
            return ComponentContainer<false, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        template <typename... TOnlyComps>
        ComponentContainer<false, TOnlyComps...> OnlyComponents()
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).OnlyComponents<TOnlyComps...>();
            return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        template <typename... TOnlyComps>
        const ComponentContainer<false, TOnlyComps...> OnlyComponents() const
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).OnlyComponents<TOnlyComps...>();
            return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        ComponentContainer<true, TComps...> EntitiesAndComponents()
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).EntitiesAndComponents();
            return ComponentContainer<true, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        const ComponentContainer<true, TComps...> EntitiesAndComponents() const
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).EntitiesAndComponents();
            return ComponentContainer<true, TComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        template <typename... TOnlyComps>
        ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents()
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).EntitiesAndOnlyComponents<TOnlyComps...>();
            return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }

        template <typename... TOnlyComps>
        const ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents() const
        {
            if (!active_scene_id.empty())
                return InAnyOf(active_scene_id).EntitiesAndOnlyComponents<TOnlyComps...>();
            return ComponentContainer<false, TOnlyComps...>(lic::Range<SceneInfo, TComps...>::entities);
        }
    };
}