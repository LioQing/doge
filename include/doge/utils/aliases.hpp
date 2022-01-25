#pragma once

#include <doge/utils/lic.hpp>
#include <doge/utils/lev.hpp>

namespace doge
{
    using DeltaTime = float;
    using EntityID = lic::EntityID;
    using ComponentID = lic::ComponentID;

    template <typename... TArgs>
    using Event = lev::Event<TArgs...>;
}