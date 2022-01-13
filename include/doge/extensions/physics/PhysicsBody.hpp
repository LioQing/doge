#pragma once

#include <doge/utils/Vec2.hpp>
#include <box2d/box2d.h>

namespace doge
{
    struct PhysicsBody
    {
        b2Body* b2_body;

        void SetTransform(const Vec2f& position, float rotation);
        Vec2f GetPosition() const;
        float GetRotation() const;

        Vec2f GetCenterOfMass() const;
        Vec2f GetLocalCenterOfMass() const;

        void SetVelocity(const Vec2f& velocity);
        Vec2f GetVelocity() const;

        void SetAngularVelocity(float angular_velocity);
        float GetAngularVelocity() const;

        float GetMass() const;
        float GetMomentOfInertia() const;

        void SetDamping(float damping);
        float GetDamping() const;

        void SetAngularDamping(float damping);
        float GetAngularDamping() const;

        void ApplyForce(const Vec2f& force);
        void ApplyForce(const Vec2f& force, const Vec2f& position);

        void ApplyAcceleration(const Vec2f& acceleration);
        void ApplyAcceleration(const Vec2f& acceleration, const Vec2f& position);

        void ApplyImpulse(const Vec2f& impulse);
        void ApplyImpulse(const Vec2f& impulse, const Vec2f& position);

        void ApplyTorque(float torque);

        void ApplyAngularAcceleration(float angular_acceleration);

        void ApplyAngularImpulse(float impulse);
    };
}