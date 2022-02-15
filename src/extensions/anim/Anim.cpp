#include <doge/extensions/anim/Anim.hpp>

#include <doge/extensions/anim/Animation.hpp>
#include <doge/core/GameLoopFunctions.hpp>

namespace doge::anim
{
    Anim::Anim(Engine& engine) : engine(engine)
    {
        GameLoopFunctions glf;
        glf.update = [&](Engine& engine, DeltaTime dt){ Update(engine, dt); };

        engine.scenes.extensions.emplace("doge_anim", glf);
    }

    Anim::~Anim()
    {
    }

    void Anim::SetPaused(bool paused)
    {
        this->paused = paused;        
    }

    bool Anim::IsPaused() const
    {
        return paused;
    }

    void Anim::Update(Engine& engine, DeltaTime dt)
    {
        if (IsPaused())
            return;
            
        for (auto [entity, anim, sprite] : engine.Select<Animation, Sprite>().EntitiesAndComponents())
        {
            if (anim.is_paused)
                continue;

            State* curr_state = &anim.states.at(anim.current_state);

            anim.frame_timer += dt;

            while (anim.frame_timer > curr_state->frame_time)
            {
                anim.frame_timer -= curr_state->frame_time;
                ++anim.current_frame;

                if (anim.current_frame >= curr_state->frame_count)
                {
                    anim.current_state = curr_state->next_state;
                    anim.current_frame = 0;
                    curr_state = &anim.states.at(anim.current_state);
                }
            }

            sprite.texture_rectangle = Recti(curr_state->first_frame_position + curr_state->advance * curr_state->frame_size * anim.current_frame, curr_state->frame_size);
        }
    }
}