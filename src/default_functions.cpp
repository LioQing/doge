#include <doge/core/default_functions.hpp>

#include <vector>
#include <doge/core/Engine.hpp>

namespace doge
{
    namespace default_functions
    {
        void Finish(Engine& e)
        {
            std::vector<Entity> to_be_destroyed;
            for (auto entity : e.Select<>().InAllOf(e.GetActiveScene()).Entities())
            {
                to_be_destroyed.push_back(entity);
            }

            for (auto entity : to_be_destroyed)
            {
                e.DestroyEntity(entity);
            }
        }
    }
}