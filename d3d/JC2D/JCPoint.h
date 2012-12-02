#ifndef __JC_POINT_H__
#define __JC_POINT_H__

#include <Windows.h>

#include "JCObject.h"

class JCPoint : public JCObject
{
public:
	JCPoint();
	JCPoint(FLOAT px, FLOAT py);
	JCPoint(CONST JCPoint& point);
	~JCPoint();

	FLOAT x, y;
};

#endif