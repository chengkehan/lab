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