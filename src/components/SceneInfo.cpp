#include <doge/components/SceneInfo.hpp>

namespace doge
{
    SceneInfo::SceneInfo(const std::vector<std::string>& scene_ids)
        : scene_ids(scene_ids)
    {
    }

    std::vector<std::string>& SceneInfo::GetSceneIDs()
    {
        is_modified = true;
        return scene_ids;
    }

    const std::vector<std::string>& SceneInfo::GetSceneIDs() const
    {
        return scene_ids;
    }
    
    std::vector<std::string>& SceneInfo::_NoModify_GetSceneIDs()
    {
        return scene_ids;
    }

    void SceneInfo::_ResetModify()
    {
        is_modified = false;
    }

    bool SceneInfo::_IsModified() const
    {
        return is_modified;
    }
}