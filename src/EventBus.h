/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include <list>
#include <unordered_map>
#include <functional>
#include <queue>
#include "IEvent.h"

// rudimentary event bus (implemented as Singelton) 
// for registering delegates that execute code, when event is triggered.
// addons for the future: abort events, timer to limit processing of
// events, ...
//
// BE CAREFUL: the EventBus has not been thoroughly tested.
class EventBus
{
public:
	using EventListener = std::function<void(IEvent::ptr)>;
	using ListenerId = unsigned long;

	static EventBus& getInstance();

	// Add/remove event listeners
	// Be careful: 
	// Due to the usage of std::function, multiple additions of the
	// same function to the m_listener cannot be checked.
	ListenerId addListener(IEvent::EventType type, EventListener listener);
	void removeListener(ListenerId id);

	// immediately triggers the event
	void fireEvent(const IEvent::ptr& event);
	// queue event to be processed
	void queueEvent(const IEvent::ptr& event);
	// should called once per frame to process events
	void processEvents(float deltaTime);

private:

	struct ListenerData
	{
		ListenerId m_id;
		EventListener m_listener;
	};

	using ListenerList = std::list<ListenerData>;
	using EventListenerMap = std::unordered_map<IEvent::EventType, ListenerList>;
	using ListenerIdEventMap = std::unordered_map<ListenerId, IEvent::EventType>;
	using EventQueue = std::queue<IEvent::ptr>;

	static const int m_numMaxQueues = 2;
	static ListenerId m_listenerIdCtr;

	int m_currentEventQueue = 0;

	// buffering the events to avoid circles when new events are
	// queued each frame before the old ones are processed.
	EventQueue m_eventQueueBuffer[m_numMaxQueues];

	// The map stores the registered listeners for each event.
	EventListenerMap m_eventListenerMap;
	// This map is used for faster lookups when removing listeners.
	ListenerIdEventMap m_listenerIdEventMap;
};
