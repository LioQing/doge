#pragma once

#include <doge/core/Engine.hpp>

namespace doge::anim
{
    struct Anim
    {
        Anim(Engine& engine);
        ~Anim();

    private:

        Engine& engine;
        
        void Start(Engine& engine);
        void Update(Engine& engine, DeltaTime dt);
        void FixedUpdate(Engine& engine, DeltaTime dt);
        void Finish(Engine& engine);
    };
}