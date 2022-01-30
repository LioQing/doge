#pragma once

#include <unordered_map>
#include <vector>
#include <string>
#include <functional>

namespace doge::lev
{
    template <typename... TArgs>
    class Event
    {
    private:

        std::unordered_map<std::string, std::function<void(TArgs...)>> listeners;
        std::vector<std::function<void(TArgs...)>> idless_listeners;

    public:

        template <std::convertible_to<std::string> TID, std::invocable<TArgs...> TListener>
        Event& AddListener(const std::pair<TID, TListener>& id_listener) &
        {
            listeners.emplace(id_listener);
            return *this;
        }

        template <std::invocable<TArgs...> TListener>
        Event& AddListener(const std::string& id, TListener listener) &
        {
            listeners.emplace(id, listener);
            return *this;
        }

        template <std::invocable<TArgs...> TListener>
        Event& AddListener(TListener listener) &
        {
            idless_listeners.push_back(listener);
            return *this;
        }

        template <std::convertible_to<std::string> TID, std::invocable<TArgs...> TListener>
        Event& operator+=(const std::pair<TID, TListener>& id_listener) &
        {
            return AddListener(id_listener);
        }

        template <std::invocable<TArgs...> TListener>
        Event& operator+=(TListener listener) &
        {
            return AddListener(listener);
        }

        Event& RemoveListener(const std::string& id) &
        {
            listeners.erase(id);
            return *this;
        }

        Event& operator-=(const std::string& id) &
        {
            return RemoveListener(id);
        }

        template <typename... UArgs>
        void Raise(UArgs&&... args) const&
        {
            for (auto& [id, function] : listeners)
            {
                function(std::forward<UArgs>(args)...);
            }

            for (auto& function : idless_listeners)
            {
                function(std::forward<UArgs>(args)...);
            }
        }

        void Raise() const&
        {
            if constexpr (sizeof...(TArgs) == 0)
            {
                for (auto& [id, function] : listeners)
                {
                    function();
                }

                for (auto& function : idless_listeners)
                {
                    function();
                }
            }
            else
            {
                static_assert(std::is_base_of<Event, std::tuple_element_t<0, std::tuple<TArgs...>>>::value,
                    "Cannot call raise event when lev::Event<TArg> is not base of TArg");

                for (auto& [id, function] : listeners)
                {
                    function(*static_cast<const std::tuple_element_t<0, std::tuple<TArgs...>>*>(this));
                }

                for (auto& function : idless_listeners)
                {
                    function(*static_cast<const std::tuple_element_t<0, std::tuple<TArgs...>>*>(this));
                }
            }
        }

        template <typename... UArgs>
        void operator()(UArgs&&... args) const&
        {
            Raise(std::forward<UArgs>(args)...);
        }

        void operator()() const&
        {
            Raise();
        }
    };
}