/// code by
/// MOSS TAMARA | fhs44413
/// PALAGIC CATALIN | fhs44416
/// FH Salzburg - MMT 2021

#pragma once

#include "IEvent.h"

#include "GameObject.h"

class GameObjectCreateEvent : public BasicDataEvent<GameObject::ptr>
{
public:
	using BasicDataEvent::BasicDataEvent;

	// EventType defined using GUID functionality of VS 2015. 
	// Tools->Create GUID -> DEFINE_GUID -> take longest value in the beginning.
	// output:
	//
	// {DEC71435-8D92-4CA7-A3D5-FD17F9D5CC34}
	// DEFINE_GUID(<< name >> ,
	//	0xdec71435, 0x8d92, 0x4ca7, 0xa3, 0xd5, 0xfd, 0x17, 0xf9, 0xd5, 0xcc, 0x34);
	//
	// m_EventType identifies event, when we want to add it to the event bus
	static const EventType eventType;
	BasicDataEvent::EventType getEventType() override { return eventType; }
};

class GameObjectDeleteEvent : public BasicDataEvent<GameObject::ptr>
{
public:
	using BasicDataEvent::BasicDataEvent;

	static const EventType eventType;
	EventType getEventType() override { return eventType; }
};