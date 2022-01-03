#pragma once

#include <vector>
#include <string>

namespace doge
{
    class SceneInfo
    {
        std::vector<std::string> scene_ids = std::vector<std::string>(); // no scene id represent all scene

        bool is_modified = true;

    public:

        SceneInfo(const SceneInfo&) = default;
        SceneInfo(const std::vector<std::string>& scene_ids = std::vector<std::string>());

        std::vector<std::string>& GetSceneIDs();
        const std::vector<std::string>& GetSceneIDs() const;

        std::vector<std::string>& _NoModify_GetSceneIDs();
        void _ResetModify();
        bool _IsModified() const;
    };
}