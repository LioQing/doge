#pragma once

#include <string>
#include <unordered_map>
#include <doge/utils/aliases.hpp>
#include <doge/core/GameLoopFunctions.hpp>

namespace doge
{
    struct GameLoopFunctions;

    struct Scenes
    {
        DeltaTime fixed_time_step = 10.f;
        std::unordered_map<std::string, GameLoopFunctions> extensions;
        std::unordered_map<std::string, GameLoopFunctions> scenes;
        std::string current_scene_id;
        std::string active_scene_id;
        bool is_open = false;
        bool is_running = false;
    };
}