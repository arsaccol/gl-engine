#pragma once

#include "EventDispatcher.hpp"
#include <functional>

template <typename EventType>
class Observer
{
protected:
	Observer();
	virtual void EventHandler(const Event&) = 0;

};


// ============= Template method implementations =============
template <typename EventType>
Observer<EventType>::Observer()
{
	auto& dispatcher = EventDispatcher::Instance();
	dispatcher.On<EventType>(
		std::bind(&Observer<EventType>::EventHandler, this, std::placeholders::_1)
	);
}
