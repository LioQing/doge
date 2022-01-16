#pragma once

#include <vector>
#include <string>

namespace doge
{
    struct EntityInfo
    {
        bool enabled = true;
        std::vector<std::string> scene_ids; // no scene id represent all scene
    };
}