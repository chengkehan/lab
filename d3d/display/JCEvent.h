#ifndef __JC_EVENT_H__
#define __JC_EVENT_H__

#include <Windows.h>

#include "JCObject.h"

class JCEvent : public JCObject
{
public:
	JCEvent();
	virtual ~JCEvent();

public:
	INT eventID;
};

#endif