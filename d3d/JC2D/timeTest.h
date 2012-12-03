#ifndef __TIME_TEST_H__
#define __TIME_TEST_H__

#include <Windows.h>
#include <cstdio>
#include <ctime>

extern DWORD timeTest1;
inline VOID timeTestStart()
{
	timeTest1 = timeGetTime();
}

inline VOID timeTestEnd()
{
	char buffer[256];
	sprintf(buffer, "time %d\n", timeGetTime() - timeTest1);
	OutputDebugStringA(buffer);
}

#endif