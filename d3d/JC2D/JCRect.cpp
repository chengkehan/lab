#include "JCRect.h"

JCRect::JCRect():left(0.0f), top(0.0f), right(0.0f), bottom(0.0f)
{

}

JCRect::JCRect(FLOAT pLeft, FLOAT pTop, FLOAT pRight, FLOAT pBottom):left(pLeft), top(pTop), right(pRight), bottom(pBottom)
{

}

JCRect::JCRect(CONST JCRect& rect)
{
	left = rect.left;
	top = rect.top;
	right = rect.right;
	bottom = rect.bottom;
}

JCRect::~JCRect()
{

}

BOOL JCRect::contains(FLOAT px, FLOAT py)
{
	return containsPoint(px, py);
}

BOOL JCRect::contains(CONST JCRect* lpRect) CONST
{
	if(lpRect == NULL)
	{
		return FALSE;
	}
	else if(lpRect == this)
	{
		return TRUE;
	}
	else
	{
		return containsPoint(lpRect->left, lpRect->top) && containsPoint(lpRect->right, lpRect->bottom);
	}
}

BOOL JCRect::intersects(CONST JCRect* lpRect) CONST
{
	if(lpRect == NULL)
	{
		return FALSE;
	}
	else if(lpRect == this)
	{
		return TRUE;
	}
	else
	{
		FLOAT x1, x2, y1, y2;
		return intersectoinInternal(lpRect, &x1, &x2, &y1, &y2);
	}
}

JCRect* JCRect::intersection(CONST JCRect* lpDestRect, JCRect* lpIntersectionResult) CONST
{
	if(lpIntersectionResult == NULL)
	{
		return NULL;
	}
	else if(lpDestRect == this)
	{
		return clone(lpIntersectionResult);
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
			lpIntersectionResult->left = x1;
			lpIntersectionResult->right = x2;
			lpIntersectionResult->top = y1;
			lpIntersectionResult->bottom = y2;
			return lpIntersectionResult;
		}
	}
}

JCRect* JCRect::combine(CONST JCRect* lpDestRect, JCRect* lpCombineResult) CONST
{
	if(lpDestRect == NULL || lpCombineResult == NULL)
	{
		return NULL;
	}
	else if(lpCombineResult == this)
	{
		return NULL;
	}
	else if(lpDestRect == this)
	{
		return clone(lpCombineResult);
	}
	else
	{
		lpCombineResult->left = min(min(left, right), min(lpDestRect->left, lpDestRect->right));
		lpCombineResult->right = max(max(left, right), max(lpDestRect->left, lpDestRect->right));
		lpCombineResult->top = min(min(top, bottom), min(lpDestRect->top, lpDestRect->bottom));
		lpCombineResult->bottom = max(max(top, bottom), max(lpDestRect->top, lpDestRect->bottom));
		return lpCombineResult;
	}
}

JCRect* JCRect::clone(JCRect* lpDestRect) CONST
{
	if(lpDestRect == NULL)
	{
		return lpDestRect;
	}
	else if(lpDestRect == this)
	{
		return lpDestRect;
	}
	else
	{
		lpDestRect->left = left;
		lpDestRect->top = top;
		lpDestRect->right = right;
		lpDestRect->bottom = bottom;

		return lpDestRect;
	}
}

CONST JCRect* JCRect::copy(CONST JCRect* lpSrcRect)
{
	if(lpSrcRect == NULL)
	{
		return NULL;
	}
	else if(lpSrcRect == this)
	{
		return lpSrcRect;
	}
	else
	{
		left = lpSrcRect->left;
		top = lpSrcRect->top;
		right = lpSrcRect->right;
		bottom = lpSrcRect->bottom;

		return lpSrcRect;
	}
}

inline BOOL JCRect::containsPoint(FLOAT px, FLOAT py) CONST
{
	return px >= left && px <= right && py >= top && py <= bottom;
}

inline BOOL JCRect::intersectoinInternal(CONST JCRect* lpRect, FLOAT* x1, FLOAT* x2, FLOAT* y1 ,FLOAT* y2) CONST
{
	*x1 = max(min(left, right), min(lpRect->left, lpRect->right));
	*x2 = min(min(left, right), min(lpRect->left, lpRect->right));
	*y1 = max(min(top, bottom), min(lpRect->top, lpRect->bottom));
	*y2 = min(min(top, bottom), min(lpRect->top, lpRect->bottom));

	return *x1 <= *x2 && *y1 <= *y2;
}