#pragma once

#include "EventDispatcher.hpp"
#include <functional>

// Inherit from this and implement void EventHandler(const Event&)
// to have an observer that will respond to EventType events.
// ============================================================
// I'm not sure this is the right way to do it. Maybe we should
// just give observers an std::function and register it with the
// EventDispatcher in the observer's constructor?

namespace event 
{

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

}
