#include <doge/utils/global.hpp>

namespace doge
{
    Vec2f global::GetPosition(EntityID eid)
    {
        return lic::GetEntity(eid).GetIfHasComponentElseDefault<Position>().position;
    }

    Vec2f global::GetScale(EntityID eid)
    {
        return lic::GetEntity(eid).GetIfHasComponentElseDefault<Scale>().scale;
    }

    float global::GetRotation(EntityID eid)
    {
        return lic::GetEntity(eid).GetIfHasComponentElseDefault<Rotation>().rotation;
    }
}