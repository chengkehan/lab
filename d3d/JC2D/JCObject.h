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

	VOID setName(LPCWCHAR name);
	LPCWCHAR getName();

	VOID setExtraData(LPVOID data);
	LPVOID getExtraData();

private:
	JCObject(CONST JCObject& object);

	VOID destroyName();

	LPWCH m_name;
	LPVOID m_extraData;
};

#endif