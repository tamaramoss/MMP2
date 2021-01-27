#pragma once

#include <memory>

class IEvent
{
public:
	IEvent() = default;
	virtual ~IEvent() = default;

	using EventType = unsigned long;
	using ptr = std::shared_ptr<IEvent>;

	virtual EventType getEventType() = 0;
};

template <typename T>
class BasicDataEvent : public IEvent
{
public:

	BasicDataEvent(T data)
		: m_data(data)
	{
	}

	~BasicDataEvent() override = default;

	T getData() { return m_data; }

private:

	T m_data;
};
