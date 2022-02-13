#pragma once

#include <string>
#include <doge/utils/Vec2.hpp>

namespace doge::anim
{
    struct State
    {
        Vec2i frame_size;
        Vec2i first_frame_position;
        std::size_t frame_count;
        float frame_time; // in milliseconds
        std::string next_state;
        
        Vec2i advance = Vec2i::Right;
    };
}