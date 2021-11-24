#include <doge/utils/lic.hpp>

namespace doge
{
    std::array<std::any, lic::LIC_MAX_COMPONENT> lic::components;
    std::array<std::vector<std::size_t>, lic::LIC_MAX_COMPONENT> lic::destroyed_components;
    std::vector<lic::EntityInfo> lic::entities;
    std::vector<lic::EntityID> lic::destroyed_entities;
    lic::ComponentID lic::next_component_id;
    lic::EntityID lic::next_entity_id;

    void lic::Entity::RemoveComponent(ComponentID cid) const
    {
        return lic::RemoveComponent(this->id, cid);
    }

    bool lic::Entity::HasComponent(ComponentID cid) const
    {
        return lic::HasComponent(this->id, cid);
    }

    lic::Entity lic::AddEntity()
    {
        if (destroyed_entities.empty())
        {
            EntityID eid = next_entity_id++;
            entities.emplace_back(eid, false);
            return Entity(entities.back().id);
        }
        else
        {
            EntityID eid = destroyed_entities.back();
            entities.at(eid) = EntityInfo(eid, false);
            destroyed_entities.pop_back();
            return Entity(entities.at(eid).id);
        }
    }

    void lic::DestroyEntity(EntityID eid)
    {
        for (ComponentID cid = 0u; cid < LIC_MAX_COMPONENT; ++cid)
        {
            if (HasComponent(eid, cid))
                RemoveComponent(eid, cid);
        }

        entities.at(eid).component_field.reset();
        destroyed_entities.push_back(eid);
    }

    lic::Entity lic::GetEntity(EntityID eid)
    {
        return Entity(entities.at(eid).id);
    }

    void lic::RemoveComponent(EntityID eid, ComponentID cid)
    {
        if (!HasComponent(eid, cid))
        {
            return;
        }

        auto& entity = entities.at(eid);
        auto cind = entities.at(eid).component_indices.at(cid);

        destroyed_components.at(cid).push_back(cind);
        entities.at(eid).component_field.set(cid, false);
    }

    bool lic::HasComponent(EntityID eid, ComponentID cid)
    {
        return entities.at(eid).component_field.test(cid);
    }

    lic::Entity lic::EntityContainer::Iterator::operator*() const
    {
        return Entity(entities.at(VecIterator::operator*()).id);
    }

    lic::EntityContainer::Iterator lic::EntityContainer::begin() const
    {
        return Iterator(std::vector<EntityID>::cbegin());
    }

    lic::EntityContainer::Iterator lic::EntityContainer::end() const
    {
        return Iterator(std::vector<EntityID>::cend());
    }

    lic::EntityContainer::Iterator lic::EntityContainer::cbegin() const
    {
        return Iterator(std::vector<EntityID>::cbegin());
    }

    lic::EntityContainer::Iterator lic::EntityContainer::cend() const
    {
        return Iterator(std::vector<EntityID>::cend());
    }
}