#pragma once

namespace event 
{
	class Event;

	// Inherit from this class, and use
	// SendEvent<YourEventTypeHere>() calls to emit events
	class Observable
	{
	public:
		template <typename EventSubType>
		static void SendEvent(const Event& event)
		{
			EventDispatcher::Instance().DispatchEvent<EventSubType>(event);
		}

		~Observable() = default;
	};
}
