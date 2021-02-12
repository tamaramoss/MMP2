/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#include "stdafx.h"

#include "EventBus.h"
#include <utility>

EventBus::ListenerId EventBus::m_listenerIdCtr = 0;

EventBus& EventBus::getInstance()
{
	static EventBus instance;
	return instance;
}

EventBus::ListenerId
EventBus::addListener(const IEvent::EventType type, EventListener listener)
{
	++m_listenerIdCtr;
	m_eventListenerMap[type].push_back({m_listenerIdCtr, std::move(listener)});
	m_listenerIdEventMap[m_listenerIdCtr] = type;
	return m_listenerIdCtr;
}

void EventBus::removeListener(const ListenerId id)
{
	const auto eventTypeIt = m_listenerIdEventMap.find(id);
	if (eventTypeIt == m_listenerIdEventMap.end())
		return;

	auto eventListenerIt = m_eventListenerMap.find(eventTypeIt->second);
	if (eventListenerIt == m_eventListenerMap.end())
	{
		FF_ERROR_MSG("Possible error: did not find event type in m_listener list.");
		return;
	}

	auto found = false; //< for debugging
	auto& listenerList = eventListenerIt->second;
	for (auto it = listenerList.begin(); it != listenerList.end(); ++it)
	{
		if ((*it).m_id != id)
			continue;
		it = listenerList.erase(it);
		m_listenerIdEventMap.erase(id);
		found = true;
		break;
	}

	if (found)
		err() << "Removed m_listener with given mID. " << id << std::endl;
	else
		err() << "Possible error: did not find m_listener with this mID for this event. "
			<< id << " " << eventTypeIt->second << std::endl;
}

void EventBus::fireEvent(const IEvent::ptr& event)
{
	auto it = m_eventListenerMap.find(event->getEventType());
	if (it == m_eventListenerMap.end())
		return;

	auto& listenerList = it->second;
	for (auto listenerData : listenerList)
	{
		listenerData.m_listener(event);
	}
}

void EventBus::queueEvent(const IEvent::ptr& event)
{
	m_eventQueueBuffer[m_currentEventQueue].push(event);
}

void EventBus::processEvents(float deltaTime)
{
	const auto current_queue = m_currentEventQueue;
	while (!m_eventQueueBuffer[current_queue].empty())
	{
		auto event = m_eventQueueBuffer[current_queue].front();
		m_eventQueueBuffer[current_queue].pop();

		fireEvent(event);
	}

	// switch buffer
	m_currentEventQueue = (current_queue + 1) % m_numMaxQueues;
}
