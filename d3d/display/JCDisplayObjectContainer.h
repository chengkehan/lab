#ifndef __JC_DISPLAYOBJECTCONTAINER_H__
#define __JC_DISPLAYOBJECTCONTAINER_H__

#include <Windows.h>
#include <list>

#include "JCDisplayObject.h"

class JCDisplayObjectContainer : public JCDisplayObject
{
public:
	JCDisplayObjectContainer();
	~JCDisplayObjectContainer();

public:
	VOID addChild(JCDisplayObject* child);
	VOID removeChild(JCDisplayObject* child);
	BOOL containsChild(JCDisplayObject* child);
	INT numChildren();

private:
	std::list<JCDisplayObject*> m_childrenList;
};

#endif