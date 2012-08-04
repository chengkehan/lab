#include <Windows.h>
#include <iostream>
#include <cstdio>

#define NUM_THREADS 10

HANDLE threads[NUM_THREADS];
DWORD threadIDs[NUM_THREADS];

VOID myIntPrint(INT value)
{
	CHAR printBuffer[1024];
	sprintf(printBuffer, "%d\n", value);
	OutputDebugStringA(printBuffer);
}

DWORD WINAPI myThread(LPVOID pValue)
{
	myIntPrint((INT)pValue);

	return (DWORD)pValue;
}

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	for (INT i = 0; i < NUM_THREADS; ++i)
	{
		threads[i] = CreateThread(NULL, 0, myThread, (LPVOID)i, 0, &threadIDs[i]);
	}

	for (INT i = 0; i < 5; ++i)
	{
		OutputDebugString(L"main thread\n");
		Sleep(2000);
	}

	for (INT i = 0 ; i < NUM_THREADS; ++i)
	{
		CloseHandle(threads[i]);
	}

	getchar();
	return 0;
}