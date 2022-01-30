#include <doge/extensions/physics/Collider.hpp>

#include <doge/extensions/physics/Physics.hpp>

namespace doge::physics
{
    void Collider::SetSensor(bool sensor)
    {
        b2_fixture->SetSensor(sensor);
    }

    bool Collider::IsSensor() const
    {
        return b2_fixture->IsSensor();
    }

    bool Collider::TestPoint(const Vec2f& point)
    {
        return b2_fixture->TestPoint(Physics::ToB2Vec2(point));
    }
}