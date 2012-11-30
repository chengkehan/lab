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
};

#endif