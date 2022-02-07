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
    struct Range : public lic::Range<TComps...>
    {
        template <typename... TSelectComps>
        Range<TComps..., TSelectComps...> Select() const
        {
            return Range<TComps..., TSelectComps...>(lic::Range<TComps...>::template Select<TSelectComps...>());
        }

        template <std::predicate<Entity, TComps...> TPred>
        Range<TComps...> Where(TPred predicate) const
        {
            return Range(lic::Range<TComps...>::Where([&](lic::Entity entity, const TComps&... c)
            { return predicate(Entity(entity, PCNode::root.GetDescendent(entity).get()), c...); }));
        }

        EntityContainer Entities() const
        {
            return EntityContainer(lic::Range<TComps...>::entities);
        }

        ComponentContainer<false, TComps...> Components()
        {
            return ComponentContainer<false, TComps...>(lic::Range<TComps...>::entities);
        }

        const ComponentContainer<false, TComps...> Components() const
        {
            return ComponentContainer<false, TComps...>(lic::Range<TComps...>::entities);
        }

        template <typename... TOnlyComps>
        ComponentContainer<false, TOnlyComps...> OnlyComponents()
        {
            return ComponentContainer<false, TOnlyComps...>(lic::Range<TComps...>::entities);
        }

        template <typename... TOnlyComps>
        const ComponentContainer<false, TOnlyComps...> OnlyComponents() const
        {
            return ComponentContainer<false, TOnlyComps...>(lic::Range<TComps...>::entities);
        }

        ComponentContainer<true, TComps...> EntitiesAndComponents()
        {
            return ComponentContainer<true, TComps...>(lic::Range<TComps...>::entities);
        }

        const ComponentContainer<true, TComps...> EntitiesAndComponents() const
        {
            return ComponentContainer<true, TComps...>(lic::Range<TComps...>::entities);
        }

        template <typename... TOnlyComps>
        ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents()
        {
            return ComponentContainer<true, TOnlyComps...>(lic::Range<TComps...>::entities);
        }

        template <typename... TOnlyComps>
        const ComponentContainer<true, TOnlyComps...> EntitiesAndOnlyComponents() const
        {
            return ComponentContainer<true, TOnlyComps...>(lic::Range<TComps...>::entities);
        }
    };
}