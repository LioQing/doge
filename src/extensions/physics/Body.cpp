#include <doge/extensions/physics/Body.hpp>

#include <doge/extensions/physics/Physics.hpp>

namespace doge::physics
{
    void Body::SetTransform(const Vec2f& position, float rotation)
    {
        b2_body->SetTransform(Physics::ToB2Vec2(position), rotation);
    }

    Vec2f Body::GetPosition() const
    {
        return Physics::FromB2Vec2(b2_body->GetPosition());
    }

    float Body::GetRotation() const
    {
        return b2_body->GetAngle();
    }

    Vec2f Body::GetCenterOfMass() const
    {
        return Physics::FromB2Vec2(b2_body->GetWorldCenter());
    }

    Vec2f Body::GetLocalCenterOfMass() const
    {
        return Physics::FromB2Vec2(b2_body->GetLocalCenter());
    }

    void Body::SetVelocity(const Vec2f& velocity)
    {
        b2_body->SetLinearVelocity(Physics::ToB2Vec2(velocity));
    }

    Vec2f Body::GetVelocity() const
    {
        return Physics::FromB2Vec2(b2_body->GetLinearVelocity());
    }

    void Body::SetAngularVelocity(float angular_velocity)
    {
        b2_body->SetAngularVelocity(angular_velocity);
    }

    float Body::GetAngularVelocity() const
    {
        return b2_body->GetAngularVelocity();
    }

    float Body::GetMass() const
    {
        return b2_body->GetMass();
    }

    float Body::GetMomentOfInertia() const
    {
        return b2_body->GetInertia();
    }

    void Body::SetDamping(float damping)
    {
        b2_body->SetLinearDamping(damping);
    }

    float Body::GetDamping() const
    {
        return b2_body->GetLinearDamping();
    }

    void Body::SetAngularDamping(float damping)
    {
        b2_body->SetAngularDamping(damping);
    }

    float Body::GetAngularDamping() const
    {
        return b2_body->GetAngularDamping();
    }

    void Body::ApplyForce(const Vec2f& force)
    {
        b2_body->ApplyForceToCenter(Physics::ToB2Vec2(force), true);
    }

    void Body::ApplyForce(const Vec2f& force, const Vec2f& position)
    {
        b2_body->ApplyForce(Physics::ToB2Vec2(force), Physics::ToB2Vec2(position), true);
    }

    void Body::ApplyAcceleration(const Vec2f& acceleration)
    {
        ApplyForce(acceleration * GetMass());
    }

    void Body::ApplyAcceleration(const Vec2f& acceleration, const Vec2f& position)
    {
        ApplyForce(acceleration * GetMass(), position);
    }

    void Body::ApplyImpulse(const Vec2f& impulse)
    {
        b2_body->ApplyLinearImpulseToCenter(Physics::ToB2Vec2(impulse), true);
    }

    void Body::ApplyImpulse(const Vec2f& impulse, const Vec2f& position)
    {
        b2_body->ApplyLinearImpulse(Physics::ToB2Vec2(impulse), Physics::ToB2Vec2(position), true);
    }

    void Body::ApplyTorque(float torque)
    {
        b2_body->ApplyTorque(torque, true);
    }

    void Body::ApplyAngularAcceleration(float angular_acceleration)
    {
        b2_body->ApplyTorque(angular_acceleration * GetMomentOfInertia(), true);
    }

    void Body::ApplyAngularImpulse(float impulse)
    {
        b2_body->ApplyAngularImpulse(impulse, true);
    }
}