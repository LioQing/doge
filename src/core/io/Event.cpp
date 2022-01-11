#include <doge/core/io/Event.hpp>

#include <doge/utils.hpp>

namespace doge::io
{
    void Event::OnPollEvent(const sf::Event& event)
    {
        this->event.Raise(cast::FromSfEvent(event));
    }
}