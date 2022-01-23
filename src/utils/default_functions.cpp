#include <doge/utils/default_functions.hpp>

#include <vector>
#include <doge/core/Engine.hpp>

namespace doge
{
    namespace default_functions
    {
        void Finish(Engine& engine)
        {
            for (auto entity : engine.Select<EntityInfo>()
                .Where([](Entity _, const EntityInfo& entity_info)
                { return entity_info.destroy_on_finish; })
                .Entities())
            {
                engine.DestroyEntity(entity);
            }
        }
    }
}