#ifndef __JC_OBJECT_H__
#define __JC_OBJECT_H__

#include <Windows.h>

#include "jccommon.h"
#include "jcstring.h"

class JCObject
{
public:
	JCObject();
	virtual ~JCObject();

public:
	VOID setName(LPCWCHAR name);
	LPCWCHAR getName();

private:
	VOID destroyName();

private:
	LPWCH m_name;
};

#endif