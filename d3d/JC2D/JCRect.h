#ifndef __JC_RECT_H__
#define __JC_RECT_H__

#include <Windows.h>

#include "JCObject.h"

class JCRect : public JCObject
{
public:
	JCRect();
	JCRect(FLOAT pLeft, FLOAT pTop, FLOAT pRight, FLOAT pBottom);
	JCRect(CONST JCRect& rect);
	~JCRect();

	FLOAT left, top, right, bottom;

	BOOL contains(FLOAT px, FLOAT py);
	BOOL contains(CONST JCRect* lpRect) CONST;
	BOOL intersects(CONST JCRect* lpRect) CONST;
	JCRect* intersection(CONST JCRect* lpDestRect, JCRect* lpIntersectionResult) CONST;
	JCRect* combine(CONST JCRect* lpDestRect, JCRect* lpCombineResult) CONST;
	JCRect* clone(JCRect* lpDestRect) CONST;
	CONST JCRect* copy(CONST JCRect* lpSrcRect);

private:
	inline BOOL containsPoint(FLOAT px, FLOAT py) CONST;
	inline BOOL intersectoinInternal(CONST JCRect* lpDestRect, FLOAT* x1, FLOAT* x2, FLOAT* y1 ,FLOAT* y2) CONST;
};

#endif