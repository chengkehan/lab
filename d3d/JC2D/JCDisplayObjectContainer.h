#ifndef __JC_DISPLAYOBJECTCONTAINER_H__
#define __JC_DISPLAYOBJECTCONTAINER_H__

#include <Windows.h>
#include <list>
#include <d3d9.h>

#include "JCDisplayObject.h"
#include "jcstring.h"
#include "jccommon.h"

class JCDisplayObjectContainer : public JCDisplayObject
{
public:
	JCDisplayObjectContainer(IDirect3DDevice9* lpd3dd);
	virtual ~JCDisplayObjectContainer();

public:
	INT getNumChildren() CONST;
	JCDisplayObject* addChild(JCDisplayObject* child);
	JCDisplayObject* addChildAt(JCDisplayObject* child, INT index);
	JCDisplayObject* removeChild(JCDisplayObject* child);
	JCDisplayObject* removeChildAt(INT index);
	BOOL containsChild(JCDisplayObject* child);
	INT getChildIndex(JCDisplayObject* child);
	JCDisplayObject* getChildAt(INT index);
	JCDisplayObject* getChildByName(LPCWCHAR name);
	JCDisplayObject* setChildIndex(JCDisplayObject* child, INT index);
	virtual VOID render();

private:
	JCDisplayObjectContainer();

protected:
	std::list<JCDisplayObject*> m_childrenList;
};

#endif