#include <doge/utils/default_functions.hpp>

#include <vector>
#include <doge/core/Engine.hpp>

namespace doge
{
    namespace default_functions
    {
        void Finish(Engine& e)
        {
            for (auto entity : e.Select<>().InAllOf(e.GetActiveScene()).Entities())
            {
                e.DestroyEntity(entity);
            }
        }
    }
}