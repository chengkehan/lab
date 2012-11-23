#include "JCObject.h"

using namespace jcstring;

JCObject::JCObject():m_name(NULL)
{

}

JCObject::~JCObject()
{
	destroyName();
}

VOID JCObject::setName(LPCWCHAR name)
{
	if(!jcstring_wcsequ(name, m_name))
	{
		destroyName();
		if(name != NULL)
		{
			SIZE_T strlen = jcstring_wcslen(name);
			// unicode char, 2 bytes, end of 2 bytes ending flag
			INT numBytes = strlen * 2 + 2;
			jccommon_mallocCleanM(numBytes, m_name, LPWCH);
			jccommon_memcpyM(m_name, name, numBytes);
		}
	}
}

LPCWCHAR JCObject::getName() CONST
{
	return m_name;
}

VOID JCObject::setExtraData(LPVOID data)
{
	m_extraData = data;
}

LPVOID JCObject::getExtraData() CONST
{
	return m_extraData;
}

VOID JCObject::destroyName()
{
	jccommon_deleteM(m_name);
}