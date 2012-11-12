#ifndef __JC_DISPLAYOBJECT_H__
#define __JC_DISPLAYOBJECT_H__

#include <Windows.h>

#include "JCEventDispatcher.h"

class JCDisplayObject : public JCEventDispatcher
{
public:
	JCDisplayObject();
	virtual ~JCDisplayObject();

private:
	FLOAT m_x;
	FLOAT m_y;
};

#endif