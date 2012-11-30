#ifndef __JC_DISPLAYOBJECTCONTAINER_H__
#define __JC_DISPLAYOBJECTCONTAINER_H__

#include <Windows.h>
#include <list>
#include <d3d9.h>
#include <climits>

#include "JCDisplayObject.h"
#include "jcstring.h"
#include "jccommon.h"

class JC2D;

class JCDisplayObjectContainer : public JCDisplayObject
{
friend class JC2D;

public:
	JCDisplayObjectContainer(IDirect3DDevice9* lpd3dd);
	virtual ~JCDisplayObjectContainer();

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

protected:
	inline VOID updateRealWHAndBounds(FLOAT parentGlobalX, FLOAT parentGlobalY);

private:
	JCDisplayObjectContainer();

	std::list<JCDisplayObject*> m_childrenList;
};

#endif