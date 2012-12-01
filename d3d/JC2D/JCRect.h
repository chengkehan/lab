#ifndef __JC_RECT_H__
#define __JC_RECT_H__

#include <Windows.h>

class JCRect
{
public:
	JCRect();
	~JCRect();
	JCRect(FLOAT px, FLOAT py, FLOAT pwidth, FLOAT pheight);
	JCRect(CONST JCRect& rect);

	FLOAT x, y, width, height;

	BOOL contains(FLOAT px, FLOAT py);
	BOOL contains(JCRect* lpRect);
	BOOL intersects(JCRect* lpRect);
	JCRect* intersection(JCRect* lpDestRect, JCRect* lpIntersectionResult);
	JCRect* combine(JCRect* lpDestRect, JCRect* lpCombineResult);

private:
	inline BOOL containsPoint(FLOAT px, FLOAT py);
	inline BOOL intersectoinInternal(JCRect* lpDestRect, FLOAT* x1, FLOAT* x2, FLOAT* y1 ,FLOAT* y2);
};

#endif