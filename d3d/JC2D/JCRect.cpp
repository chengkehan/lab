#include "JCRect.h"

JCRect::JCRect():x(0.0f), y(0.0f), width(0.0f), height(0.0f)
{

}

JCRect::~JCRect()
{

}

JCRect::JCRect(FLOAT px, FLOAT py, FLOAT pwidth, FLOAT pheight):x(px), y(py), width(pwidth), height(pheight)
{

}

JCRect::JCRect(CONST JCRect& rect)
{
	x = rect.x;
	y = rect.y;
	width = rect.width;
	height = rect.height;
}

BOOL JCRect::contains(FLOAT px, FLOAT py)
{
	return containsPoint(px, py);
}

BOOL JCRect::contains(JCRect* lpRect)
{
	if(lpRect == NULL)
	{
		return FALSE;
	}
	else
	{
		return containsPoint(lpRect->x, lpRect->y) && containsPoint(lpRect->x + lpRect->width, lpRect->y + lpRect->height);
	}
}

BOOL JCRect::intersects(JCRect* lpRect)
{
	if(lpRect == NULL)
	{
		return FALSE;
	}
	else
	{
		FLOAT x1, x2, y1, y2;
		return intersectoinInternal(lpRect, &x1, &x2, &y1, &y2);
	}
}

JCRect* JCRect::intersection(JCRect* lpDestRect, JCRect* lpIntersectionResult)
{
	if(lpIntersectionResult == NULL)
	{
		return NULL;
	}
	else
	{
		FLOAT x1, x2, y1, y2;
		BOOL r = intersectoinInternal(lpDestRect, &x1, &x2, &y1, &y2);
		if(!r)
		{
			return NULL;
		}
		else
		{
			lpIntersectionResult->x = x1;
			lpIntersectionResult->width = x2 - x1;
			lpIntersectionResult->y = y1;
			lpIntersectionResult->height = y2 - y1;
			return lpIntersectionResult;
		}
	}
}

JCRect* JCRect::combine(JCRect* lpDestRect, JCRect* lpCombineResult)
{
	if(lpDestRect == NULL || lpCombineResult == NULL)
	{
		return NULL;
	}
	else
	{
		lpCombineResult->x = min(min(x, x + width), min(lpDestRect->x, lpDestRect->x + lpDestRect->width));
		lpCombineResult->width = max(max(x, x + width), max(lpDestRect->x, lpDestRect->x + lpDestRect->width)) - lpCombineResult->x;
		lpCombineResult->y = min(min(y, y + height), min(lpDestRect->y, lpDestRect->y + lpDestRect->height));
		lpCombineResult->height = max(max(y, y + height), max(lpDestRect->y, lpDestRect->y + lpDestRect->height)) - lpCombineResult->y;
		return lpCombineResult;
	}
}

inline BOOL JCRect::containsPoint(FLOAT px, FLOAT py)
{
	return px >= x && px <= x + width && py >= y && py <= y + height;
}

inline BOOL JCRect::intersectoinInternal(JCRect* lpRect, FLOAT* x1, FLOAT* x2, FLOAT* y1 ,FLOAT* y2)
{
	*x1 = max(min(x, x + width), min(lpRect->x, lpRect->x + lpRect->width));
	*x2 = min(min(x, x + width), min(lpRect->x, lpRect->x + lpRect->width));
	*y1 = max(min(y, y + height), min(lpRect->y, lpRect->y + lpRect->height));
	*y2 = min(min(y, y + height), min(lpRect->y, lpRect->y + lpRect->height));

	return *x1 <= *x2 && *y1 <= *y2;
}