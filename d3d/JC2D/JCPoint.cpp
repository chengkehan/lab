#include "JCPoint.h"

JCPoint::JCPoint():x(0.0f), y(0.0f)
{

}

JCPoint::JCPoint(FLOAT px, FLOAT py)
{
	x = px;
	y = py;
}

JCPoint::JCPoint(CONST JCPoint& point)
{
	x = point.x;
	y = point.y;
}

JCPoint::~JCPoint()
{

}