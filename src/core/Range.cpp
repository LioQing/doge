#include <doge/core/Range.hpp>

#include <doge/core/PCNode.hpp>

namespace doge
{
    Entity EntityContainer::Iterator::operator*() const
    {
        EntityID eid = lic::GetEntity(VecIterator::operator*()).id;
        return Entity(eid, PCNode::root.GetDescendent(eid).get());
    }

    EntityContainer::Iterator EntityContainer::begin() const
    {
        return Iterator(std::vector<EntityID>::cbegin());
    }

    EntityContainer::Iterator EntityContainer::end() const
    {
        return Iterator(std::vector<EntityID>::cend());
    }

    EntityContainer::Iterator EntityContainer::cbegin() const
    {
        return Iterator(std::vector<EntityID>::cbegin());
    }

    EntityContainer::Iterator EntityContainer::cend() const
    {
        return Iterator(std::vector<EntityID>::cend());
    }
}