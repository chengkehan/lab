#ifndef __JC_EVENTDISPATCHER_H__
#define __JC_EVENTDISPATCHER_H__

#include <Windows.h>
#include <map>
#include <list>
#include <algorithm>

#include "JCObject.h"
#include "JCEvent.h"
#include "jccommon.h"

class JCEventDispatcher : public JCObject
{
public:
	JCEventDispatcher();
	virtual ~JCEventDispatcher();

	typedef VOID(*EventHandler)(JCEvent* lpEvent);

	BOOL addEventListener(INT eventID, EventHandler handler);
	BOOL removeEventListener(INT eventID, EventHandler handler);
	BOOL hasEventListener(INT eventID);
	BOOL dispatchEvent(JCEvent* lpEvent);

private:
	JCEventDispatcher(CONST JCEventDispatcher& eventDispatcher);

	typedef std::list<EventHandler> HandlerList;
	typedef std::map<INT, HandlerList*> HandlersMap;

	// key eventID, value handlers
	HandlersMap m_eventHandlers;
};

#endif