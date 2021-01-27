#pragma once

#include <utility>
#include "IEvent.h"

#include "SpriteRenderComponent.h"
#include "TileLayerRenderComponent.h"

// RigidBodyComponent is a reference, because components are managed by
// game objects
class RenderableCreateEvent : public BasicDataEvent<IRenderComponent&>
{
public:
	RenderableCreateEvent(std::string layer, IRenderComponent& comp)
		: BasicDataEvent(comp)
		  , m_layer(std::move(layer))
	{
	}

	// EventType defined using GUID functionality of VS 2015. 
	// Tools->Create GUID -> DEFINE_GUID -> take longest value in the beginning.
	// output:
	//
	// {DEC71435-8D92-4CA7-A3D5-FD17F9D5CC34}
	// DEFINE_GUID(<< name >> ,
	//	0xdec71435, 0x8d92, 0x4ca7, 0xa3, 0xd5, 0xfd, 0x17, 0xf9, 0xd5, 0xcc, 0x34);
	//
	// m_EventType identifies event, when we want to add it to the event bus
	static const EventType m_eventType;
	EventType getEventType() override { return m_eventType; }

	std::string getLayer() const { return m_layer; }

private:
	std::string m_layer;
};
