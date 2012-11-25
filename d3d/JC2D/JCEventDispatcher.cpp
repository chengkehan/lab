#include "JCEventDispatcher.h"

using namespace std;

JCEventDispatcher::JCEventDispatcher()
{

}

JCEventDispatcher::~JCEventDispatcher()
{
	for(HandlersMap::iterator iterMap = m_eventHandlers.begin(); iterMap != m_eventHandlers.end(); ++iterMap)
	{
		HandlerList* list = iterMap->second;
		jccommon_deleteM(list);
	}
}

BOOL JCEventDispatcher::addEventListener(INT eventID, EventHandler handler)
{
	if(handler == NULL)
	{
		return FALSE;
	}

	HandlersMap::iterator iter = m_eventHandlers.find(eventID);
	if(iter != m_eventHandlers.end())
	{
		iter->second->push_back(handler);
		return TRUE;
	}
	else
	{
		HandlerList* lpHandlerList = NULL;
		jccommon_newM(lpHandlerList, HandlerList);
		lpHandlerList->push_back(handler);
		m_eventHandlers[eventID] = lpHandlerList;
		return TRUE;
	}
}

BOOL JCEventDispatcher::removeEventListener(INT eventID, EventHandler handler)
{
	if(handler == NULL)
	{
		return FALSE;
	}

	for(HandlersMap::iterator iterMap = m_eventHandlers.begin(); iterMap != m_eventHandlers.end(); ++iterMap)
	{
		if(iterMap->first == eventID)
		{
			for(HandlerList::iterator iterList = iterMap->second->begin(); iterList != iterMap->second->end(); ++iterList)
			{
				if(*iterList == handler)
				{
					iterMap->second->erase(iterList);
					break;
				}
			}
		}
	}
	return TRUE;
}

BOOL JCEventDispatcher::hasEventListener(INT eventID)
{
	HandlersMap::iterator iter = m_eventHandlers.find(eventID);
	return iter != m_eventHandlers.end();
}

BOOL JCEventDispatcher::dispatchEvent(JCEvent* lpEvent)
{
	if(lpEvent == NULL)
	{
		return FALSE;
	}

	for(HandlersMap::iterator iterMap = m_eventHandlers.begin(); iterMap != m_eventHandlers.end(); ++iterMap)
	{
		if(iterMap->first == lpEvent->eventID)
		{
			for (HandlerList::iterator iterList = iterMap->second->begin(); iterList != iterMap->second->end(); ++iterList)
			{
				((EventHandler)*iterList)(lpEvent);
			}
			break;
		}
	}
	return TRUE;
}