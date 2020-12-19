#pragma once

class Event;

class Observable
{
public:
	template <typename EventSubType>
	void SendEvent(const Event& event)
	{
		EventDispatcher::Instance().DispatchEvent<EventSubType>(event);
	}

	~Observable() = default;
};
