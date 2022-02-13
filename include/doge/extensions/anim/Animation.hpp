#pragma once

#include <doge/extensions/anim/State.hpp>
#include <unordered_map>

namespace doge::anim
{
    struct Animation
    {
        std::unordered_map<std::string, State> states;

        std::string current_state;
        
        std::size_t current_frame = 0;
        float frame_timer = 0.f;
        bool is_paused = false;

        void SetState(const std::string& new_state)
        {
            current_state = new_state;
            current_frame = 0;
            frame_timer = 0.f;
        }
    };
}