#ifndef __JC_EVENT_H__
#define __JC_EVENT_H__

#include <Windows.h>

#include "JCObject.h"

class JCEvent : public JCObject
{
public:
	JCEvent(INT pEventID);
	virtual ~JCEvent();

public:
	INT eventID;

private:
	JCEvent(CONST JCEvent& jcEvent);
};

#endif